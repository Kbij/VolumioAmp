/*
 * GpioListenerIf.h
 */

#ifndef DEVICECONTROL_GPIOLISTENERIF_H_
#define DEVICECONTROL_GPIOLISTENERIF_H_

#include <stdint.h>

namespace HardwareNs
{

class  GpioListenerIf
{
public:
	virtual ~GpioListenerIf() {};
	virtual void gpioLow(uint8_t gpio) = 0;
	virtual void gpioHigh(uint8_t gpio) = 0;
};

} /* namespace DeviceControlNs */

#endif /* DEVICECONTROL_GPIOLISTENERIF_H_ */
