#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Drivers{

class I2Cifc {
public:
	I2Cifc() = default;
	~I2Cifc() = default;
	virtual void Write(const uint16_t& addres, const std::vector<uint8_t>& data) = 0;
	virtual const std::vector<uint8_t> Read(const uint16_t& addres, const unsigned int size) = 0;
};

std::shared_ptr<I2Cifc> makeI2C(const std::string& devicePath, const uint8_t addr);

}