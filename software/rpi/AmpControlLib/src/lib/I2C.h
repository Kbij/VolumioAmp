/*
 * I2C.h
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_
#include "I2CIf.h"
#include <mutex>

namespace HardwareNs
{

class I2C: public I2CIf
{
public:
	explicit I2C(const std::string& i2cDevice);
	virtual ~I2C();

	bool writeData(uint8_t address, const std::vector<uint8_t>& data);
	bool readWriteData(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData);

private:
	const std::string mI2CDevice;
	std::mutex mBusMutex;

	bool readWriteDataWithRetry(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData, int retryCount);
	bool readWriteDataNoRetry(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData);
};

} /* namespace DeviceControlNs */

#endif /* SRC_I2C_H_ */
