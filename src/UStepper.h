#ifndef _USTEPPER_H
#define _USTEPPER_H

#include <Arduino.h>
#include "UStepperIO.h"
#include <functional>
#include <vector>

#ifndef DEFAULT_EXTENT
#define DEFAULT_EXTENT 1000
#endif

class UStepper
{
public:
    static const uint16_t MAX_STEPS = 2000;
    enum Direction : int8_t {
        FORWARD = 1,
        REVERSE = -1
    };

    enum StepType {
        HALF_STEP = 7,
        FULL_STEP = 3
    };

    using ustepper_progress_t = std::function<void()>;
    using ustepper_finished_t = std::function<void()>;
    using ustepper_step_t     = std::function<void(Direction direction)>;


    UStepper(UStepperIO& gpio, StepType step_type = HALF_STEP, uint32_t step_delay = 1);
    virtual ~UStepper();
    bool begin(const char* name = "UStepper");
    bool isLimit(Direction direction);
    uint16_t getPosition();
    float_t getPercent();
    bool step(Direction direction, uint32_t steps, ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms);
    bool percent(float_t percent, ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms);
    bool position(uint16_t position, ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms);
    bool sync(ustepper_progress_t progress, ustepper_finished_t finished, uint32_t wait_ms);
    bool off(ustepper_finished_t finished, uint32_t wait_ms);
    void onStep(ustepper_step_t step_cb);
    void task();

private:
    enum class Command : uint8_t
    {
        STOP     = 0,
        STEP     = 1,
        POSITION = 2,
        SYNC     = 3,
        OFF      = 4
    };

    typedef struct command_message
    {
        Command     command;
        Direction   direction;
        union {
            uint16_t steps;
            uint16_t position;
        } value;
        ustepper_progress_t progress;
        ustepper_finished_t finished;
    } CommandMessage;

    UStepperIO&     _gpio;
    
    StepType           _step_type;
    uint32_t           _step_delay;
    uint8_t            _step_index;
    volatile bool      _active;
    volatile uint16_t  _position;
    uint16_t           _extent;
    TaskHandle_t       _task;
    QueueHandle_t      _command_queue;
    std::vector<ustepper_step_t> _on_step;

    void process(CommandMessage& message);
    void cmdSync(ustepper_progress_t progress);
    void cmdPosition(uint16_t position, ustepper_progress_t progress);
    void cmdStep(Direction direction, uint16_t steps, ustepper_progress_t progress);
    bool step(Direction direction);
    bool queueCommand(CommandMessage* message, uint32_t wait_ms);

};

#endif // _USTEPPER_H
