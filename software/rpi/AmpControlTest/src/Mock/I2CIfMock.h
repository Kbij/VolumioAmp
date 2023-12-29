/*
** VolumioAmp
** File description:
** I2CIfMock
*/

#ifndef I2CIFMOCK_H_
#define I2CIFMOCK_H_
#include "lib/I2CIf.h"
#include <gmock/gmock.h>


class I2CIfMock: public HardwareNs::I2CIf
{
public:
	MOCK_METHOD(bool, writeData, (uint8_t address, const std::vector<uint8_t>& data));
	MOCK_METHOD(bool, readWriteData, (uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData));
};

#endif /* !I2CIFMOCK_H_ */
