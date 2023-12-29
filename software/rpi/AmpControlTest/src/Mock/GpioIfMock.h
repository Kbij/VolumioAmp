/*
** VolumioAmp
** File description:
** GpioIfMock
*/

#ifndef GPIOIFMOCK_H_
#define GPIOIFMOCK_H_
#include <gmock/gmock.h>
#include "lib/GpioIf.h"

class  GpioIfMock: public HardwareNs::GpioIf
{
public:
	MOCK_METHOD(void, registerGpioListener, (HardwareNs::GpioListenerIf* listener));
	MOCK_METHOD(void, unRegisterGpioListener, (HardwareNs::GpioListenerIf* listener));
	MOCK_METHOD(void, registerInputGpio, (uint8_t gpio, const std::string& name));
	MOCK_METHOD(void, unRegisterInputGpio, (uint8_t gpio));
	MOCK_METHOD(void, registerOutputGpio, (uint8_t gpio));
	MOCK_METHOD(void, unRegisterOutputGpio, (uint8_t gpio));
	MOCK_METHOD(void, setGpio, (uint8_t gpio, bool value));
	MOCK_METHOD(bool, getGpio, (uint8_t gpio));
};

#endif /* !GPIOIFMOCK_H_ */
