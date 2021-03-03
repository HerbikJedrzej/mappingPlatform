#pragma once

#include <string>
#include <vector>
#include <memory>
#include <logSystem.hh>

namespace Drivers{

class SPIifc {
public:
	SPIifc()= default;
	~SPIifc()= default;
	virtual bool transfer(uint8_t* dataSend, uint8_t* dataRecive, const unsigned int& length) = 0;
};

std::shared_ptr<SPIifc> makeSPI(const std::string& device);

}