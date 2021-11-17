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
#include "ConfigPortal.h"
#include "elephant.h"
#include "elephant_profile.h"
#include "version.h"

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
static ConfigPortal cp;

static const uint16_t elephant_step = 5;
static uint16_t       elephant_walking_buffer_sz = max(elephant_profile_width, elephant_profile_height) + 2*elephant_step;
static TFT_eSprite    elephant_walking(&tft);
static TFT_eSprite    elephant_walking_buffer(&tft);
static TFT_eSprite    elephant_looking(&tft);
static int16_t        easter_x;
static uint32_t       easter_look;

enum class State {
  IDLE,
  STARTING,
  STEEPING,
  STOPPING,
  SAVER,
  SPLASH,
  EASTER,
  CONFIG
};

static volatile State    state        = State::SPLASH;
static volatile time_t   state_time   = 0;
static volatile uint32_t steep_time   = 300;
static volatile bool     clear_screen = false;
static volatile bool     syncing      = true;

static const char* stateName(State s)
{
  switch (s)
  {
    case State::IDLE:     return "IDLE";
    case State::STARTING: return "STARTING";
    case State::STEEPING: return "STEEPING";
    case State::STOPPING: return "STOPPING";
    case State::SAVER:    return "SAVER";
    case State::SPLASH:   return "SPLASH";
    case State::EASTER:   return "EASTER";
    case State::CONFIG:   return "CONFIG";
    default:              return "UNKNOWN";
  }
}

static void setState(State s, bool clear = false)
{
  dlog.info(TAG, "setState: %s -> %s", stateName(state), stateName(s));
  state = s;
  state_time = time(nullptr);
  if (clear)
  {
    clear_screen = true;
  }
}

static void startSteep(bool clean_screen = false)
{
  dlog.info(TAG, "startSteep");
  setState(State::STARTING, clean_screen);
  m.step(UStepper::FORWARD, 200, nullptr, [](){
    if (state == State::STARTING)
    {
      m.off(nullptr, 1000);
      setState(State::STEEPING);
    }
  }, 1000);
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

static void displaySplash()
{
  uint16_t green  = tft.color565(0, 255, 0);
  uint16_t orange = tft.color565(255, 165, 0);

  tft.setCursor(0, 2, 4);
  tft.setTextColor(green, TFT_BLACK);
  tft.println("TeaSteeper");
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(BUILD_VERSION);
  tft.println();
  tft.print("For ");
  tft.setTextColor(orange, TFT_BLACK);
  tft.println("Liz Liebman");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Christmas 2021");
}

static void displayConfig()
{
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 2, 4);
  tft.println("Config Active");
  tft.setTextFont(2);
  tft.println(BUILD_VERSION);
  tft.setTextFont(4);
  tft.setTextColor(tft.color565(0, 255, 0), TFT_BLACK);
  tft.println("SSID: TeaSteeper");
}

static void startEaster()
{
  easter_x    = 0;
  easter_look = 0;
  setState(State::EASTER, true);
}

static void displayEaster()
{
  if (easter_look != 0)
  {
    if (millis()-easter_look > 2000)
    {
      easter_look = 0;
      startSteep(true);
      return;
    }
    easter_x = TFT_HEIGHT-elephant_width;
    elephant_looking.pushSprite(easter_x, 0);
    return;
  }

  int16_t y = (TFT_WIDTH-elephant_walking_buffer_sz)/2;
  int16_t tilt = (easter_x/elephant_step)&1 ? 5 : -5;
  elephant_walking_buffer.fillSprite(TFT_BLACK);
  elephant_walking.pushRotated(&elephant_walking_buffer, tilt);
  elephant_walking_buffer.pushSprite(easter_x-elephant_profile_width, y);
  easter_x += elephant_step;
  if (easter_x > TFT_HEIGHT+elephant_profile_width/2)
  {
    easter_x = 0;
    easter_look = millis();
  }
  delay(90);
}

static void displayTask(void* data)
{
  (void)data;
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
        displayTime(steep_time, tft.color565(255, 165, 0));
        break;
      case State::STEEPING:
      {
        time_t now = time(nullptr);
        uint32_t time_left = steep_time - (now - state_time);
        displayTime(time_left, tft.color565(0, 255, 0));
        break;
      }
      case State::STOPPING:
        displayTime(0, tft.color565(255, 165, 0));
        break;
      case State::SAVER:
        screensaverLife(&tft);
        break;
      case State::SPLASH:
        displaySplash();
        break;
      case State::EASTER:
        displayEaster();
        //elephant_looking.pushSprite(0, 0);
        //tft.setPivot(120, 67);
        //elephant_walking.pushRotated(45);
        break;

      case State::CONFIG:
        displayConfig();
        break;
    }
    delay(10);
  }
}

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
  dlog.info(TAG, "displayInit: create display task");
  xTaskCreatePinnedToCore(displayTask, "display", 4096, nullptr, 1, nullptr, 1);
}

void setup()
{
  Serial.begin(115200);
  dlog.begin(new DLogPrintWriter(Serial));
  delay(200);
  dlog.info(TAG, "setup: Starting!");

  displayInit();

  // initialize sprites
  dlog.info(TAG, "setup: initialize sprites");
  elephant_looking.createSprite(elephant_width, elephant_height);
  elephant_looking.setSwapBytes(true);
  elephant_looking.pushImage(0, 0, elephant_width, elephant_height, elephant);
  elephant_walking.setPivot(elephant_width/2, elephant_height/2);

  elephant_walking.createSprite(elephant_width, elephant_height);
  elephant_walking.setSwapBytes(true);
  elephant_walking.pushImage(0, 0, elephant_profile_width, elephant_profile_height, elephant_profile);
  elephant_walking.setPivot(elephant_profile_width/2, elephant_profile_height/2);

  elephant_walking_buffer.createSprite(elephant_walking_buffer_sz, elephant_walking_buffer_sz);
  elephant_walking_buffer.setPivot(elephant_walking_buffer_sz/2, elephant_walking_buffer_sz/2);


  dlog.info(TAG, "setup: initialze button and encoder");
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
  delay(2000); // make sure the splash screen is visable for a few seconds
  bool start_config = digitalRead(PIN_ROTSW) == 0;
  if (start_config)
  {
    cp.startPortal("TeaSteeper");
  }
  setState(start_config ? State::CONFIG : State::IDLE, true);
  dlog.info(TAG, "setup: done");
}

void loop()
{
  cp.poll();
  button.read();
  // if the screen saver or easter is on ignore the encoder and 
  // go back to idle when the button is pressed
  if (state == State::SAVER || state == State::EASTER)
  {
    if (button.wasReleased())
    {
      dlog.info(TAG, "loop: stop screensaver");
      setState(State::IDLE, true);
      return;
    }
    delay(1);
    return;
  }

  encoder->tick();

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

  time_t now = time(nullptr);
  if (button.pressedFor(5000))
  {
    dlog.info(TAG, "loop: RESETTING!!!!!!");
    delay(100);
    ESP.restart();
  }
  switch(state)
  {
    case State::IDLE:
    {
      // detect that arm was pushed off home position and correct.
      if (!io.isUpperLimit())
      {
        dlog.info(TAG, "loop: arm was physically moved, correcting!");
        setState(State::STOPPING);
        m.step(UStepper::FORWARD, 50, nullptr, [](){
          m.step(UStepper::REVERSE, 500, nullptr, [](){
            dlog.info(TAG, "loop: state now IDLE");
            m.off(nullptr, 1000);
            setState(State::IDLE);
          }, 1000);
        }, 1000);
      }
      if (button.wasReleased())
      {
        if (steep_time % 60 == 42)
        {
          dlog.info(TAG, "loop: 42! starting easter egg!");
          startEaster();
          break;
        }
        startSteep();
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
