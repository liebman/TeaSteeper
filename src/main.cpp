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

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
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
static RotaryEncoder *encoder;
static TFT_eSPI tft;

enum class State {
  IDLE,
  STARTING,
  STEEPING,
  STOPPING
};

static volatile State    state      = State::IDLE;
static volatile time_t   state_time = 0;
static volatile uint32_t steep_time = 60;

static bool syncing = true;

static void displayInit()
{
  dlog.info(TAG, "displayInit");
  //Set up the display
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
}

static void displayTime(uint32_t value)
{
  uint8_t minutes = value / 60;
  uint8_t seconds = value % 60;
  tft.setCursor(0, 0, 8);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.printf("%01d:%02d", minutes, seconds);
}

static void displayTask(void* data)
{
  (void)data;
  State  last_state  = state;
  time_t last_update = 0;
  dlog.info(TAG, "displayTask");
  while(true)
  {
    switch (state)
    {
      case State::IDLE:
        displayTime(steep_time);
        break;
      case State::STARTING:
        displayTime(steep_time);
        break;
      case State::STEEPING:
      {
        time_t now = time(nullptr);
        uint32_t time_left = steep_time - (now - state_time);
        displayTime(time_left);
        break;
      }
      case State::STOPPING:
        displayTime(steep_time);
        break;
    }
    delay(10);
  }
}

void setup()
{
  Serial.begin(115200);
  dlog.begin(new DLogPrintWriter(Serial));
  delay(200);
  dlog.info(TAG, "setup: Starting!");

  displayInit();
  button.begin();
  encoder = new RotaryEncoder(PIN_ROTB, PIN_ROTA, RotaryEncoder::LatchMode::FOUR3);
  encoder->setPosition(steep_time);

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
  dlog.info(TAG, "setup: create display task");
  xTaskCreatePinnedToCore(displayTask, "display", 4096, nullptr, 1, nullptr, 1);
  dlog.info(TAG, "setup: done");
}

static void setState(State s)
{
  state = s;
  state_time = time(nullptr);
}

void loop()
{
  encoder->tick();
  button.read();

  long pos = encoder->getPosition();
  if (pos < 0)
  {
    pos = 0;
    encoder->setPosition(pos);
  } else if (pos > 599)
  {
    pos = 599;
    encoder->setPosition(pos);
  }

  if (pos != steep_time)
  {
    steep_time = pos;
  }

  if (state == State::IDLE)
  {
    if (button.isPressed())
    {
      dlog.info(TAG, "loop: state now STARTING");
      setState(State::STARTING);
      m.step(UStepper::FORWARD, 350, nullptr, [](){
        dlog.info(TAG, "loop: state now STEEPING");
        m.off(nullptr, 1000);
        state_time = time(nullptr);
        setState(State::STEEPING);
      }, 1000);
    }
  }
  else if (state == State::STEEPING)
  {
    time_t now = time(nullptr);
    uint32_t time_left = steep_time - (now - state_time);
    if (button.isPressed() || time_left<1)
    {
      dlog.info(TAG, "loop: state now STOPPING");
      setState(State::STOPPING);
      m.step(UStepper::REVERSE, 500, nullptr, [](){
        dlog.info(TAG, "loop: state now IDLE");
        m.off(nullptr, 1000);
        state_time = 0;
        setState(State::IDLE);
      }, 1000);
    }
  }

  delay(1);
}
