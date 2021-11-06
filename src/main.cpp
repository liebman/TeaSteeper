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

extern void screensaverLife(TFT_eSPI* tft);

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
  STOPPING,
  SAVER
};

static volatile State    state        = State::IDLE;
static volatile time_t   state_time   = 0;
static volatile uint32_t steep_time   = 60;
static volatile bool     clear_screen = false;
static volatile bool     syncing      = true;

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

static void displayTime(uint32_t value, uint16_t color)
{
  char buf[8];
  uint8_t minutes = value / 60;
  uint8_t seconds = value % 60;
  tft.setTextColor(color, TFT_BLACK);
  tft.setTextFont(8);
  snprintf(buf, sizeof(buf)-1, "%01d:%02d", minutes, seconds);
  int16_t tw = tft.textWidth(buf);
  int32_t x = (TFT_HEIGHT - tw) / 2;
  int32_t y = (TFT_WIDTH - 75) / 2;
  tft.drawString(buf, x, y);
}

static void displayTask(void* data)
{
  (void)data;
  State  last_state  = state;
  time_t last_update = 0;
  dlog.info(TAG, "displayTask");
  while(true)
  {
    if (clear_screen)
    {
      clear_screen = false;
      tft.fillScreen(TFT_BLACK);
    }

    switch (state)
    {
      case State::IDLE:
        displayTime(steep_time, TFT_WHITE);
        break;
      case State::STARTING:
        displayTime(steep_time, TFT_GREEN);
        break;
      case State::STEEPING:
      {
        time_t now = time(nullptr);
        uint32_t time_left = steep_time - (now - state_time);
        displayTime(time_left, TFT_WHITE);
        break;
      }
      case State::STOPPING:
        displayTime(0, TFT_RED);
        break;
      case State::SAVER:
        screensaverLife(&tft);
        break;
    }
    delay(10);
  }
}

static void setState(State s, bool clear = false)
{
  state = s;
  state_time = time(nullptr);
  if (clear)
  {
    clear_screen = true;
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
  setState(State::IDLE);
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

  if (state == State::SAVER)
  {
    if (pos != steep_time || button.wasPressed())
    {
      dlog.info(TAG, "loop: stop screensaver");
      setState(State::IDLE, true);
      encoder->setPosition(steep_time);
      return;
    }
  }

  if (pos != steep_time)
  {
    steep_time = pos;
  }

  time_t now = time(nullptr);
  
  switch(state)
  {
    case State::IDLE:
    {
      if (button.wasPressed())
      {
        if (steep_time == 42)
        {
          dlog.info(TAG, "loop: 42! starting screensaver!");
          setState(State::SAVER, true);
          break;
        }
        dlog.info(TAG, "loop: state now STARTING");
        setState(State::STARTING);
        m.step(UStepper::FORWARD, 350, nullptr, [](){
          if (state == State::STARTING)
          {
            dlog.info(TAG, "loop: state now STEEPING");
            m.off(nullptr, 1000);
            setState(State::STEEPING);
          }
        }, 1000);
      }
      else
      {
        // screen saver starts after 5 minutes idle
        if ((now - state_time) > 300)
        {
          dlog.info(TAG, "loop: idle! starting screensaver!");
          setState(State::SAVER, true);
          break;
        }
      }
      break;
    }
    case State::STARTING:
    case State::STEEPING:
    {
      uint32_t time_left = steep_time - (now - state_time);
      if (button.wasPressed() || time_left<1)
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
      break;
    }
    case State::STOPPING:
      break;
  }

  delay(1);
}
