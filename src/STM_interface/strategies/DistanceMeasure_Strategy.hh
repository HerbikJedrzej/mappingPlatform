#pragma once
#include "StrategySPI_ifc.hh"

namespace StrategySpi{

class DistanceMeasureStrategy : public StrategySpiIfc{
    double* distance;
public:
    DistanceMeasureStrategy(double* _distance);
    bool set(StrategySpiData& data) override;
    bool get(StrategySpiData& data) override;
    unsigned int getSize() override;
    std::string getName() override;
};

}