/*
** VolumioAmp
** File description:
** MA12040P
*/

#ifndef MA12040P_H_
#define MA12040P_H_
#include "GpioListenerIf.h"

namespace HardwareNs
{
struct MA12040PSettings
{

};
class GpioIf;
class I2CIf;

class MA12040P: public GpioListenerIf
{
public:
    MA12040P(const MA12040PSettings& settings, GpioIf* gpio, I2CIf* i2c);
    virtual ~MA12040P();

    //GpioListenerIf
	void gpioLow(uint8_t gpio);
	void gpioHigh(uint8_t gpio);

private:
    const MA12040PSettings mSettings;
    GpioIf* mGpio;
    I2CIf* mI2c;
};
}
#endif /* !MA12040P_H_ */
