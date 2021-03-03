#include "Battery_Strategy.hh"
#include <stdexcept>

namespace StrategySpi{

BatteryStrategy::BatteryStrategy(
        std::vector<double>* _actualPercentValue,
        std::vector<double>* _tuneVoltageValue
    ):
    actualPercentValue(_actualPercentValue),
    tuneVoltageValue(_tuneVoltageValue),
    numberOfCells(tuneVoltageValue->size()),
    currentUsedCell(0){
        if(tuneVoltageValue->size() != actualPercentValue->size())
            throw std::invalid_argument(
                "tuneVoltageValue size ["
                + std::to_string(tuneVoltageValue->size())
                + "] is different than actualPercentValue size ["
                + std::to_string(actualPercentValue->size()) + "]");
        if(tuneVoltageValue->size() > 8)
            throw std::out_of_range("Max number of battery cells could be 7. Acctual is "
                + std::to_string(tuneVoltageValue->size()));
    }

bool BatteryStrategy::set(StrategySpiData& data){
    uint16_t rawValue = 0;
    if((tuneVoltageValue->at(currentUsedCell)) >= 40.96)
        rawValue = 0x1fff;
    else if((tuneVoltageValue->at(currentUsedCell)) > 0.0)
        rawValue = uint16_t(tuneVoltageValue->at(currentUsedCell) * 200.0) & 0x1fff;
    rawValue |= (currentUsedCell << 13) & 0x0e000;

    data.data[0] = uint8_t((rawValue >> 8) & 0xff);
    data.data[1] = uint8_t(rawValue & 0xff);

    currentUsedCell++;
    if(currentUsedCell >= numberOfCells)
        currentUsedCell = 0;
    data.dataSize -= 2;
    data.data += 2;
    return true;
}

bool BatteryStrategy::get(StrategySpiData& data){
    uint16_t rawData = (data.data[0] << 8) | data.data[1];
    uint8_t cell = (data.data[0] >> 5) & 0b111;
    if(cell >= numberOfCells)
        throw std::out_of_range("Get information about cell ["
            + std::to_string(cell) + "]. Max allowed is 7.");
    actualPercentValue->at(cell) = double(rawData & 0x1fff) * 0.005;
    data.dataSize -= 2;
    data.data += 2;
    return true;
}

unsigned int BatteryStrategy::getSize(){
    return 2;
}

std::string BatteryStrategy::getName(){
    return "BatteryStrategy";
}

}