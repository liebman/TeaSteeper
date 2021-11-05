/* 
 * MIT License
 *
 * Copyright (c) 2021 Christopher B. Liebman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Arduino.h"
#include "Log.h"
#include "DLogPrintWriter.h"
#include "UStepperGPIO.h"
#include "UStepper.h"
#include "EasyButton.h"
#include "RotaryEncoder.h"

static const char* TAG = "main";
static const uint8_t PIN_ULIM = 4;
static const uint8_t PIN_M1A = 25;
static const uint8_t PIN_M1B = 26;
static const uint8_t PIN_M1C = 27;
static const uint8_t PIN_M1D = 32;
static const uint8_t PIN_ROTSW = 2;
static const uint8_t PIN_ROTA  = 19;
static const uint8_t PIN_ROTB  = 16;

DLog& dlog = DLog::getLog();

static UStepperGPIO io(PIN_M1A, PIN_M1B, PIN_M1C, PIN_M1D, PIN_ULIM);
static UStepper m(io);
static EasyButton button(PIN_ROTSW);
static RotaryEncoder encoder(PIN_ROTB, PIN_ROTA);

enum class State {
  IDLE,
  STARTING,
  STEEPING,
  STOPPING
};

static State    state      = State::IDLE;
static uint32_t steep_time = 60;
static time_t   start_time = 0;

static bool syncing = true;

void setup()
{
  Serial.begin(115200);
  dlog.begin(new DLogPrintWriter(Serial));
  delay(200);
  dlog.info(TAG, "setup: Starting!");

  button.begin();
  encoder.setPosition(steep_time);

  dlog.info(TAG, "setup: io begin");
  io.begin();
  dlog.info(TAG, "setup: m begin");
  m.begin();
  dlog.info(TAG, "setup: sync begin");
  m.sync(nullptr, [](){
    dlog.info(TAG, "m.sync done!");
    syncing = false;
    }, 1000);
  // wait for sync to be done.
  while(syncing)
  {
    delay(100);
  }
  dlog.info(TAG, "setup: limit=%s", io.isUpperLimit() ? "true" : "false");
  m.off([](){
    dlog.info(TAG, "m.off done!");
    }, 1000);
  dlog.info(TAG, "setup: done");
}

void loop()
{
  encoder.tick();
  button.read();
  long pos = encoder.getPosition();
  if (pos < 0)
  {
    pos = 0;
    encoder.setPosition(0);
  }
  if (pos != steep_time)
  {
    steep_time = pos;
    dlog.info(TAG, "loop: steep time = %lu", steep_time);
  }

  if (state == State::IDLE)
  {
    if (button.isPressed())
    {
      dlog.info(TAG, "loop: state now STARTING");
      state = State::STARTING;
      m.step(UStepper::FORWARD, 350, nullptr, [](){
        dlog.info(TAG, "loop: state now STEEPING");
        m.off(nullptr, 1000);
        start_time = time(nullptr);
        state = State::STEEPING;
      }, 1000);
    }
  }
  else if (state == State::STEEPING)
  {
    static time_t last_update = 0;
    time_t now = time(nullptr);
    uint32_t time_left = steep_time - (now - start_time);
    if (now > last_update)
    {
      dlog.info(TAG, "loop: STEEPING time left = %ld", time_left);
      last_update = now;
    }
    if (button.isPressed() || time_left<1)
    {
      dlog.info(TAG, "loop: state now STOPPING");
      state = State::STOPPING;
      m.step(UStepper::REVERSE, 500, nullptr, [](){
        dlog.info(TAG, "loop: state now IDLE");
        m.off(nullptr, 1000);
        start_time = 0;
        state = State::IDLE;
      }, 1000);
    }
  }
  delay(1);
}
