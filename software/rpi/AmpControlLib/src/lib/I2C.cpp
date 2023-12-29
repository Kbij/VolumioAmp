/*
 * I2C.cpp
 *
 */

#include "I2C.h"
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string>
#include <iomanip>
#include <string.h>
#include <exception>
#include <stdio.h>
#include <glog/logging.h>

namespace HardwareNs
{

I2C::I2C(const std::string& i2cDevice):
	mI2CDevice(i2cDevice),
	mBusMutex()
{
#ifndef _WIN32
	int i2cFile;
	// Open port for reading and writing
	if ((i2cFile = open(mI2CDevice.c_str(), O_RDWR)) < 0)
	{
		LOG(ERROR) << "Failed to open bus (" << mI2CDevice << "): " << strerror(errno);
	}

	close(i2cFile);
#endif
}

I2C::~I2C()
{
}

bool I2C::writeData(uint8_t address, const std::vector<uint8_t>& data)
{
	std::vector<uint8_t> readData;
	return readWriteDataWithRetry(address, data, readData, 0);
}

bool I2C::readWriteData(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData)
{
	return readWriteDataWithRetry(address, writeData, readData, 0);
}

bool I2C::readWriteDataWithRetry(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData, int retryCount)
{

	int retry = -1;
	bool result = false;
	while (!result && (retry < retryCount))
	{
		if (retry > -1)
		{
			LOG(ERROR) << "Retry (address: " << (int) address << ", retrycount =" << retry;
		}
		result = readWriteDataNoRetry(address, writeData, readData);
		++retry;
	}
	return result;
}

bool I2C::readWriteDataNoRetry(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData)
{
#ifndef _WIN32
    std::lock_guard<std::mutex> lk_guard(mBusMutex);

	if (writeData.size() == 0)
	{
		LOG(ERROR) << "Writing data with 0 size (address: " << (int) address << ")";
		return false;
	}
	std::ostringstream writeLogStream;
	bool writeFirst = true;
	for (auto byte: writeData)
	{
		if (!writeFirst)
		{
			writeLogStream << ", ";
		}
		writeFirst = false;

		writeLogStream << std::hex << std::uppercase <<  std::setfill('0') << std::setw(2) << " 0x" << (int) byte;
	}
	VLOG(3) << "Writing I2C (address: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) address << "): Data:" << writeLogStream.str() << ";";

	int i2cFile;
	if ((i2cFile = open(mI2CDevice.c_str(), O_RDWR)) < 0)
	{
		LOG(ERROR) << "Failed to open bus";
		return false;
	}

	// Set the port options and set the address of the device we wish to speak to
	if (ioctl(i2cFile, I2C_SLAVE, address) < 0)
	{
		LOG(ERROR) << "Failed setting address (address: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) address << "):" << strerror(errno);

		close(i2cFile);
		return false;
	}

	if (writeData.size() > 0)
	{
		if ((write(i2cFile, writeData.data(), writeData.size())) != static_cast<int>(writeData.size()) )
		{
			LOG(ERROR) << "Failed writing data (address: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) address << "): " << strerror(errno);

			close(i2cFile);
			return false;
		}
	}

	if (readData.size() > 0)
	{
		if (read(i2cFile, readData.data(), readData.size()) != static_cast<int>(readData.size()))
		{
			LOG(ERROR) << "Failed reading data (address: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) address << "): " << strerror(errno);

			close(i2cFile);
			return false;
		}

		std::ostringstream writeLogStreamRead;
		bool readFirst = true;
		for (auto byte: readData)
		{
			if (!readFirst)
			{
				writeLogStreamRead << ", ";
			}
			readFirst = false;

			writeLogStreamRead << std::hex << std::uppercase <<  std::setfill('0') << std::setw(2) << " 0x" << (int) byte;
		}
		VLOG(3) << "Data read (address: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) address << "):" << writeLogStreamRead.str() << ";";

	}

	close(i2cFile);
#else
	(void) address;
	(void) writeData;
	(void) readData;
#endif
	return true;
}

} /* namespace DeviceControlNs */
