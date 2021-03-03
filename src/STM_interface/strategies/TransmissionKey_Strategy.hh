#pragma once
#include "StrategySPI_ifc.hh"
#include <logSystem.hh>

namespace StrategySpi{

class TransmissionKeyStrategy : public StrategySpiIfc{
    const uint8_t keyR;
    const uint8_t keyT;
    Log log;
public:
    TransmissionKeyStrategy(const uint8_t& keyRecived, const uint8_t& keyTransmitted);
    bool set(StrategySpiData& data) override;
    bool get(StrategySpiData& data) override;
    unsigned int getSize() override;
    std::string getName() override;
};

}