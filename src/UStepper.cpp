#include "UStepper.h"
#include "Log.h"

static const char* TAG = "UStepper";

#ifndef USTEPPER_TASK_PRI
#define USTEPPER_TASK_PRI 3
#endif

#ifndef USTEPPER_TASK_CORE
#define USTEPPER_TASK_CORE ARDUINO_RUNNING_CORE
#endif

static uint8_t half_steps[8] = {
    0b0001, // { LOW,  LOW,  LOW,  HIGH }, // 0
    0b0011, // { LOW,  LOW,  HIGH, HIGH }, // 1
    0b0010, // { LOW,  LOW,  HIGH, LOW  }, // 2
    0b0110, // { LOW,  HIGH, HIGH, LOW  }, // 3
    0b0100, // { LOW,  HIGH, LOW,  LOW  }, // 4
    0b1100, // { HIGH, HIGH, LOW,  LOW  }, // 5
    0b1000, // { HIGH, LOW,  LOW,  LOW  }, // 6
    0b1001  // { HIGH, LOW,  LOW,  HIGH }, // 7
};

static uint8_t full_steps[8] = {
    0b1001, // { HIGH, LOW,  LOW,  HIGH }, // 3
    0b0011, // { LOW,  LOW,  HIGH, HIGH }, // 2
    0b0110, // { LOW,  HIGH, HIGH, LOW  }, // 1
    0b1100, // { HIGH, HIGH, LOW,  LOW  }, // 0
};

static void taskStart(void* data)
{
    UStepper* stepper = (UStepper*)data;
    stepper->task();
}

UStepper::UStepper(UStepperIO& gpio, StepType step_type, uint32_t step_delay)
: _gpio(gpio),
  _step_type(step_type),
  _step_delay(step_delay),
  _extent(DEFAULT_EXTENT)
{
}

UStepper::~UStepper()
{
}

bool UStepper::begin(const char* name)
{
    _command_queue = xQueueCreate(1, sizeof(CommandMessage));
    if (_command_queue == 0)
    {
        return false;
    }
    xTaskCreatePinnedToCore(&taskStart, name, 4096, this, 2, &_task, 1);
    if (_task == 0)
    {
        return false;
    }
    return true;
}

uint16_t UStepper::getPosition()
{
    return _position;
}

float_t UStepper::getPercent()
{
    return (float_t)_position / (float_t)_extent;
}

bool UStepper::step(Direction direction, uint32_t steps, ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms)
{
    CommandMessage cmd;
    cmd.command     = Command::STEP;
    cmd.direction   = direction;
    cmd.value.steps = steps;
    cmd.progress    = progress;
    cmd.finished    = finished;
    return queueCommand(&cmd, wait_ms);
}

bool UStepper::percent(float_t percent, ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms)
{
    if (_extent == 0)
    {
        Serial.printf("UStepper::percent: FAILING!  sync not done!\n");
        return false;
    }
    uint16_t pos = (uint16_t)(_extent * percent + 0.5);
    return position(pos, progress, finished, wait_ms);
}

bool UStepper::position(uint16_t position, ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms)
{
    CommandMessage cmd;
    cmd.command        = Command::POSITION;
    cmd.value.position = position;
    cmd.progress       = progress;
    cmd.finished       = finished;
    return queueCommand(&cmd, wait_ms);
}

bool UStepper::sync(ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms)
{
    CommandMessage cmd;
    cmd.command        = Command::SYNC;
    cmd.progress       = progress;
    cmd.finished       = finished;
    return queueCommand(&cmd, wait_ms);
}

bool UStepper::off(ustepper_finished_t finished, uint32_t wait_ms)
{
    CommandMessage cmd;
    cmd.command        = Command::OFF;
    cmd.progress       = nullptr;
    cmd.finished       = finished;
    return queueCommand(&cmd, wait_ms);
}

bool UStepper::queueCommand(CommandMessage* message, uint32_t wait_ms)
{
    if (xQueueSendToBack(_command_queue, message, pdMS_TO_TICKS(wait_ms)) != pdTRUE)
    {
        return false;
    }
    return true;
}

void UStepper::onStep(ustepper_step_t on_step)
{
    _on_step.push_back(on_step);
}

void UStepper::task()
{
    CommandMessage message;
    while(true)
    {
        if (xQueueReceive(_command_queue, &message, pdMS_TO_TICKS(100)))
        {
            process(message);
        }
    }
}

void UStepper::process(CommandMessage& message)
{
    _active = true;

    switch (message.command)
    {
    case Command::STEP:
        cmdStep(message.direction, message.value.steps, message.progress);
        break;

    case Command::POSITION:
        cmdPosition(message.value.position, message.progress);
        break;

    case Command::SYNC:
        cmdSync(message.progress);
        break;

    case Command::OFF:
        _gpio.off();
        break;

    default:
        Serial.printf("UStepper::process: unknown command:0x%d\n", (int)message.command);
        break;
    }

    _active = false;

    if(message.finished)
    {
        message.finished();
    }
}

void UStepper::cmdSync(ustepper_progress_t progress)
{
    Serial.printf("UStepper::cmdSync: starting!\n");
    //
    // if we are at teh reverse limit then step away
    //
    if (_gpio.isUpperLimit())
    {
        for(uint8_t attempt = 1; attempt <= 5; ++attempt)
        {
            dlog.info(TAG, "::cmdSync: at limit, stepping FORWARD a bit (attempt=%u)", attempt);
            cmdStep(FORWARD, 50, progress);
            if (!_gpio.isUpperLimit())
            {
                break;
            }
        }
        if (_gpio.isUpperLimit())
        {
            dlog.error(TAG, "::cmdSync: failed to move away from limit!");
            return;
        }
    }

    //
    // find the reverse limit
    //
    dlog.info(TAG, "::cmdSync: now REVERSING to find the limit");
    cmdStep(REVERSE,  MAX_STEPS, progress);
    if (!_gpio.isUpperLimit())
    {
        dlog.error(TAG, "UStepper::cmdSync: failed to find REVERSE limit!");
        return;
    }
    _position = 0;
}

void UStepper::cmdPosition(uint16_t position, ustepper_progress_t progress)
{
    int16_t delta = position - _position;
    uint16_t steps = abs(delta);
    Direction direction = delta < 0 ? REVERSE : FORWARD;
    cmdStep(direction, steps, progress);
}

void UStepper::cmdStep(Direction direction, uint16_t steps, ustepper_progress_t progress)
{
    CommandMessage maybe_stop;
    dlog.info(TAG, "::cmdStep direction=%d, steps=%u", direction, steps);

    for (uint16_t steps_taken = 0; steps_taken < steps; ++steps_taken)
    {
        // bail on any subsiquent command waiting!
        if (xQueuePeek(_command_queue, &maybe_stop, 0))
        {
            dlog.warning(TAG, "::cmdStep aborting due to queued command!");
            return;
        }

        // no steps if we are at limit!
        if (direction == REVERSE && _gpio.isUpperLimit())
        {
            dlog.info(TAG, "::cmdStep not stepping as we are at limit after %u steps!", steps_taken);
            _position = 0;
            return;
        }

        if (!step(direction))
        {
            return;
        }

        if (progress)
        {
            progress();
        }

        for(ustepper_step_t f : _on_step)
        {
            f(direction);
        }
    }
}


bool UStepper::step(Direction direction)
{
    uint8_t* steps = _step_type == FULL_STEP ? full_steps : half_steps;
    for(uint8_t i = 0; i <= _step_type; i++)
    {
        if (!_gpio.set(steps[_step_index]))
        {
            dlog.error(TAG, "::step failed mid-step! index: %u", i);
        }
        _step_index = (_step_index - (int)direction) & _step_type;
        delay(_step_delay);
    }

    // only adjust position if we are not going to overflow value
    if (direction != REVERSE || _position != 0)
    {
        _position += direction;
    }
    return true;
}
