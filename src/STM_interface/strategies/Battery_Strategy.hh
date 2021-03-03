#pragma once
#include <vector>
#include "StrategySPI_ifc.hh"

namespace StrategySpi{

class BatteryStrategy : public StrategySpiIfc{
    std::vector<double>* actualPercentValue;
    std::vector<double>* tuneVoltageValue;
    const unsigned int numberOfCells;
    unsigned int currentUsedCell;
public:
    BatteryStrategy(
        std::vector<double>* _actualPercentValue,
        std::vector<double>* _tuneVoltageValue
    );
    bool set(StrategySpiData& data) override;
    bool get(StrategySpiData& data) override;
    unsigned int getSize() override;
    std::string getName() override;
};

}