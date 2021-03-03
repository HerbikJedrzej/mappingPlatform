#pragma once

#include <string>
#include <memory>

namespace Drivers{

class GPIOifc{
public:
	GPIOifc() = default;
	GPIOifc(GPIOifc&) = default;
	GPIOifc(GPIOifc&&) = default;
	~GPIOifc() = default;
	virtual bool state() = 0;
    virtual operator bool() = 0;
    virtual void operator=(const bool& state) = 0;
};

std::shared_ptr<GPIOifc> makeGPIO(const unsigned int& pinNr, const bool& isOutput);

}