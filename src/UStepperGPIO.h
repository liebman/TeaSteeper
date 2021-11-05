#ifndef _USTEPPER_TMIO_H
#define _USTEPPER_TMIO_H

#include <stdint.h>
#include "UStepperIO.h"

class UStepperGPIO : public UStepperIO
{
public:
    UStepperGPIO(uint8_t pin_a, uint8_t pin_b, uint8_t pin_c, uint8_t pin_d, uint8_t pin_ulim);
    bool begin();

    virtual bool set(uint8_t step); // apply the step to the motor
    virtual bool off();             // all disabled
    virtual bool isUpperLimit();    // true if limit detected

private:
    uint8_t        _pin_a;
    uint8_t        _pin_b;
    uint8_t        _pin_c;
    uint8_t        _pin_d;
    uint8_t        _pin_ulim;
    bool           _on;
};

#endif // _USTEPPER_TMIO_H
