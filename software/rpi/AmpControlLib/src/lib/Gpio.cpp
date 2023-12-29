/*
 * Gpio.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: qbus
 */

#include "Gpio.h"
#include "GpioListenerIf.h"
#include "Utilities.h"
#include <fstream>
#include <glog/logging.h>
#include <iostream>

namespace
{
const std::string SYSFS_GPIO_DIR = "/sys/class/gpio";
const int GPIO_POLLING_INTERVAL_MS = 100;
const int GPIO_LONG_ACTIVE_TIME_SEC = 10;
}

namespace HardwareNs
{

Gpio::Gpio():
	mListeners(),
	mInputPins(),
	mDataMutex(),
	mThreadRunning(false),
	mThread(nullptr)
{
	startPollThread();
}

Gpio::~Gpio()
{
	stopPollThread();
}

void Gpio::registerGpioListener(GpioListenerIf* listener)
{
	std::lock_guard<std::mutex> lg(mDataMutex);
	if (listener)
	{
		mListeners.insert(listener);
	}
}

void Gpio::unRegisterGpioListener(GpioListenerIf* listener)
{
	std::lock_guard<std::mutex> lg(mDataMutex);
	if (listener)
	{
		mListeners.erase(listener);
	}
}

void Gpio::registerInputGpio(uint8_t gpio, const std::string& name)
{
	std::lock_guard<std::mutex> lg(mDataMutex);
	mInputPins[gpio].mHighCount = 0;
	mInputPins[gpio].mLowCount = 0;
	mInputPins[gpio].mName = name;

	gpioExport(gpio);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	gpioSetDirection(gpio, "in");
	gpioSetActiveLow(gpio);
	gpioSetPullUp(gpio);
}

void Gpio::unRegisterInputGpio(uint8_t gpio)
{
	std::lock_guard<std::mutex> lg(mDataMutex);
	mInputPins.erase(gpio);
	gpioUnExport(gpio);
}

void Gpio::registerOutputGpio(uint8_t gpio)
{
	gpioExport(gpio);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	gpioSetDirection(gpio, "out");
}

void Gpio::unRegisterOutputGpio(uint8_t gpio)
{
	gpioUnExport(gpio);
}

void Gpio::setGpio(uint8_t gpio, bool value)
{
	std::lock_guard<std::mutex> lg(mDataMutex);
	return writeGpio(gpio, value);
}

bool Gpio::getGpio(uint8_t gpio)
{
	std::lock_guard<std::mutex> lg(mDataMutex);
	return readGpio(gpio);
}

void Gpio::gpioExport(uint8_t gpio)
{
	try
	{
		std::ofstream exportFile(SYSFS_GPIO_DIR + "/export");
		if (exportFile.is_open())
		{
			exportFile << (int) gpio << std::endl;
		}
		else
		{
			LOG(ERROR) << "Failed exporting gpio: " << gpio;
		}
	}
	catch(std::exception& ex)
	{
		LOG(ERROR) << "Exception (" <<__FUNCTION__ << ")" << ex.what();
	}
}

void Gpio::gpioUnExport(uint8_t gpio)
{
	try
	{
		std::ofstream exportFile(SYSFS_GPIO_DIR + "/unexport");
		if (exportFile.is_open())
		{
			exportFile << (int) gpio;
		}
		else
		{
			LOG(ERROR) << "Failed un-exporting gpio: " << (int) gpio;
		}
	}
	catch(std::exception& ex)
	{
		LOG(ERROR) << "Exception (" <<__FUNCTION__ << ")" << ex.what();
	}

}

void Gpio::gpioSetDirection(uint8_t gpio, const std::string& direction)
{
	try
	{
		std::stringstream gpioFileName;
		gpioFileName << SYSFS_GPIO_DIR << "/gpio" << (int) gpio << "/direction";
		std::ofstream directionFile(gpioFileName.str());
		if (directionFile.is_open())
		{
			directionFile << direction << std::endl;
		}
		else
		{
			LOG(ERROR) << "Failed changing direction of gpio: " << (int) gpio << ", to: " << direction << "(file: " << gpioFileName.str() << ")";
		}
	}
	catch(std::exception& ex)
	{
		LOG(ERROR) << "Exception (" <<__FUNCTION__ << ")" << ex.what();
	}
}

void Gpio::gpioSetActiveLow(uint8_t gpio)
{
	try
	{
		std::stringstream gpioFileName;
		gpioFileName << SYSFS_GPIO_DIR << "/gpio" << (int) gpio << "/active_low";
		std::ofstream directionFile(gpioFileName.str());
		if (directionFile.is_open())
		{
			directionFile << 1 << std::endl;
		}
		else
		{
			LOG(ERROR) << "Failed changing active_low of gpio: " << (int) gpio << " (file: " << gpioFileName.str() << ")";
		}
	}
	catch(std::exception& ex)
	{
		LOG(ERROR) << "Exception (" <<__FUNCTION__ << ")" << ex.what();
	}
}

void Gpio::gpioSetPullUp(uint8_t gpio)
{
	std::stringstream cmd;
	cmd << "raspi-gpio set " << (int)gpio << " pu";
	runShell(cmd.str());
}

bool Gpio::readGpio(uint8_t gpio)
{
	try
	{
		std::stringstream gpioFileName;
		gpioFileName << SYSFS_GPIO_DIR << "/gpio" << (int) gpio << "/value";
		std::ifstream valueFile(gpioFileName.str());
		if (valueFile.is_open())
		{
			int value = 0;
			valueFile >> value;
			return value == 1;
		}
		else
		{
			LOG(ERROR) << "Failed reading value of gpio: " << (int) gpio ;
		}
	}
	catch(std::exception& ex)
	{
		LOG(ERROR) << "Exception (" <<__FUNCTION__ << ")" << ex.what();
	}

	return false;
}

void Gpio::writeGpio(uint8_t gpio, bool value)
{
	try
	{
		std::stringstream gpioFileName;
		gpioFileName << SYSFS_GPIO_DIR << "/gpio" << (int) gpio << "/value";
		std::ofstream valueFile(gpioFileName.str());
		if (valueFile.is_open())
		{
			valueFile << value;
		}
		else
		{
			LOG(ERROR) << "Failed writing value of gpio: " << (int) gpio ;
		}
	}
	catch(std::exception& ex)
	{
		LOG(ERROR) << "Exception (" <<__FUNCTION__ << ")" << ex.what();
	}
}


void Gpio::startPollThread()
{
	if (!mThreadRunning)
	{
		mThreadRunning = true;
		mThread = new std::thread(&Gpio::pollThread, this);
	}
}

void Gpio::stopPollThread()
{
	mThreadRunning = false;

    if (mThread)
    {
    	mThread->join();
    	delete mThread;
    	mThread = nullptr;
    }
}

void Gpio::pollThread()
{
#ifndef _WIN32
	pthread_setname_np(pthread_self(), "Gpio Polling");
#endif
	while (mThreadRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(GPIO_POLLING_INTERVAL_MS));
		std::lock_guard<std::mutex> lg(mDataMutex);
		for(auto& gpio: mInputPins)
		{
			bool value = readGpio(gpio.first);
			// if (value)
			// { // Value = true; button is not pressed
			// 	if (gpio.second.mLowCount > 0)
			// 	{//If previous value was low; button is released
			// 		if (gpio.second.mLowCount > (GPIO_LONG_ACTIVE_TIME_SEC * 1000 / GPIO_POLLING_INTERVAL_MS))
			// 		{//Long pressed
			// 			LOG(INFO) << "Gpio: " << gpio.second.mName << " (" << (int) gpio.first << ") was pressed long";
			// 			for(auto listener : mListeners)
			// 			{
			// 				listener->gpioLongActive(gpio.first);
			// 			}
			// 		}
			// 		else
			// 		{//Short pressed
			// 			LOG(INFO) << "Gpio: " << gpio.second.mName << " (" << (int) gpio.first << ") was pressed short";
			// 			for(auto listener : mListeners)
			// 			{
			// 				listener->gpioShortActive(gpio.first);
			// 			}
			// 		}
			// 	}
			// 	gpio.second.mHighCount++;
			// 	gpio.second.mLowCount = 0;
			// }
			// else
			// {// Value = false; button is pressed
			// 	gpio.second.mHighCount = 0;
			// 	gpio.second.mLowCount++;
			// }
		}
	}
}
} /* namespace DeviceControlNs */
