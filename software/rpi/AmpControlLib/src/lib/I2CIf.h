/*
 * I2CIf.h
 */

#ifndef DEVICECONTROL_I2CIF_H_
#define DEVICECONTROL_I2CIF_H_
#include <stdint.h>
#include <vector>


namespace HardwareNs
{

class I2CIf
{
public:
	virtual ~I2CIf() {};
	virtual bool writeData(uint8_t address, const std::vector<uint8_t>& data) = 0;
	virtual bool readWriteData(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData) = 0;
};
}


#endif /* DEVICECONTROL_I2CIF_H_ */
