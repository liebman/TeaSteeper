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
#include "UStepperGPIO.h"
#include "Log.h"

static const char* TAG = "UStepperIO";

UStepperGPIO::UStepperGPIO(uint8_t pin_a, uint8_t pin_b, uint8_t pin_c, uint8_t pin_d, uint8_t pin_ulim)
 : _pin_a(pin_a),
   _pin_b(pin_b),
   _pin_c(pin_c),
   _pin_d(pin_d),
   _pin_ulim(pin_ulim),
   _on(false)
{
}

bool UStepperGPIO::begin()
{
    dlog.info(TAG, "::begin pin_a=%u pin_b=%u pin_c=%u pin_d=%u ulim=%u");
    digitalWrite(_pin_a, 0);
    digitalWrite(_pin_b, 0);
    digitalWrite(_pin_c, 0);
    digitalWrite(_pin_d, 0);
    pinMode(_pin_a, OUTPUT);
    pinMode(_pin_b, OUTPUT);
    pinMode(_pin_c, OUTPUT);
    pinMode(_pin_d, OUTPUT);

    pinMode(_pin_ulim, INPUT_PULLUP);
    return true;
}

bool UStepperGPIO::set(uint8_t step)
{
    uint8_t a = step & 0b1;
    uint8_t b = step>>1 & 0b1;
    uint8_t c = step>>2 & 0b1;
    uint8_t d = step>>3 & 0b1;
    dlog.debug(TAG, "::set: step=0x%02x a=%d b=%d c=%d d=%d", step, a, b, c, d);
    digitalWrite(_pin_a, a);
    digitalWrite(_pin_b, b);
    digitalWrite(_pin_c, c);
    digitalWrite(_pin_d, d);
    return true;
}

bool UStepperGPIO::off()
{
    dlog.info(TAG, "::off()");

    return set(0);
}

bool UStepperGPIO::isUpperLimit()
{
    // 0 is limit, non-zero not limit
    uint8_t value = digitalRead(_pin_ulim);
    //dlog.info(TAG, "::isUpperLimit=%s", value==0 ? "true" : "false");
    return value == 0;
}
