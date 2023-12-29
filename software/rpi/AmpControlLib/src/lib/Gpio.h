/*
 * Gpio.h
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include "GpioIf.h"
#include <set>
#include <stdint.h>
#include <mutex>
#include <map>
#include <thread>
#include <stdint.h>

namespace HardwareNs
{
struct GpioState
{
	GpioState(): mName(), mHighCount(0), mLowCount(0){};
	std::string mName;
	int mHighCount;
	int mLowCount;
};

class Gpio: public GpioIf
{
public:
	Gpio();
	Gpio(const Gpio&) = delete;
	Gpio& operator=(const Gpio&) =delete;

	virtual ~Gpio();

	void registerGpioListener(GpioListenerIf* listener);
	void unRegisterGpioListener(GpioListenerIf* listener);

	void registerInputGpio(uint8_t gpio, const std::string& name);
	void unRegisterInputGpio(uint8_t gpio);

	void registerOutputGpio(uint8_t gpio);
	void unRegisterOutputGpio(uint8_t gpio);

	void setGpio(uint8_t gpio, bool value);
	bool getGpio(uint8_t gpio);
private:
	void gpioExport(uint8_t gpio);
	void gpioUnExport(uint8_t gpio);
	void gpioSetDirection(uint8_t gpio, const std::string& direction);
	void gpioSetActiveLow(uint8_t gpi);
	void gpioSetPullUp(uint8_t gpi);
	bool readGpio(uint8_t gpio);
	void writeGpio(uint8_t gpio, bool value);

	void startPollThread();
	void stopPollThread();
	void pollThread();

	std::set<GpioListenerIf*> mListeners;
	std::map<uint8_t, GpioState> mInputPins;
	std::mutex mDataMutex;
	bool mThreadRunning;
	std::thread* mThread;
};

} /* namespace DeviceControlNs */

#endif /* SRC_GPIO_H_ */
