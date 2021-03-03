#pragma once
#include <cstdint>
#include <string>

namespace StrategySpi{

struct StrategySpiData{
    uint8_t* data = {nullptr};
    unsigned int dataSize = {0};
};

class StrategySpiIfc{
public:
    virtual bool set(StrategySpiData& data) = 0;
    virtual bool get(StrategySpiData& data) = 0;
    virtual unsigned int getSize() = 0;
    virtual std::string getName() = 0;
};

}