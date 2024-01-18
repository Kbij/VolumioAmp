/*
 * GpioIf.h
 */

#ifndef GPIOIF_H_
#define GPIOIF_H_
#include "GpioListenerIf.h"
#include <stdint.h>
#include <string>

namespace HardwareNs
{
class GpioListenerIf;

class  GpioIf
{
public:
	virtual ~GpioIf() {};
	virtual void registerGpioListener(GpioListenerIf* listener) = 0;
	virtual void unRegisterGpioListener(GpioListenerIf* listener) = 0;

	virtual void registerInputGpio(uint8_t gpio, const std::string& name) = 0;
	virtual void unRegisterInputGpio(uint8_t gpio) = 0;

	virtual void registerOutputGpio(uint8_t gpio) = 0;
	virtual void unRegisterOutputGpio(uint8_t gpio) = 0;

	virtual void setGpio(uint8_t gpio, bool value) = 0;
	virtual bool getGpio(uint8_t gpio) = 0;
};

} /* namespace DeviceControlNs */


#endif /* GPIOIF_H_ */
