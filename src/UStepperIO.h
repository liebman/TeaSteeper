#ifndef _USTEPPER_IO_H
#define _USTEPPER_IO_H

/**
 * abstraction so that we can use unipolar stepper with MCU
 * GPIO pins or some GPIO expander.
 */

#include <stdint.h>

class UStepperIO
{
public:
    virtual bool set(uint8_t step) = 0; // apply the step to the motor lower 4 bits are stepper phase signals
    virtual bool off() = 0;             // all disabled
    virtual bool isUpperLimit() = 0;  // true if limit detected
};

#endif // _USTEPPER_IO_H
