#pragma once
#include "StrategySPI_ifc.hh"

namespace StrategySpi{

class DoubleStrategy : public StrategySpiIfc{
    double* setValue;
    double* getValue;
    union Converter{
        uint8_t array[8];
        double val;
    } converter;
public:
    DoubleStrategy(
        double* _setValue,
        double* _getValue
    );
    bool set(StrategySpiData& data) override;
    bool get(StrategySpiData& data) override;
    unsigned int getSize() override;
    std::string getName() override;
};

}