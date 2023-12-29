/*
** VolumioAmp
** File description:
** MA12040P
*/

#include "MA12040P.h"

namespace HardwareNs
{
MA12040P::MA12040P(const MA12040PSettings& settings, GpioIf* gpio, I2CIf* i2c):
    mSettings(),
    mGpio(),
    mI2c()
{
}

MA12040P::~MA12040P()
{
}

void MA12040P::gpioLow(uint8_t gpio)
{
}

void MA12040P::gpioHigh(uint8_t gpio)
{
}

}
