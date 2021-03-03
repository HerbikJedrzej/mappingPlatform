#include "DistanceMeasure_Strategy.hh"

namespace StrategySpi{

DistanceMeasureStrategy::DistanceMeasureStrategy(double* _distance):
    distance(_distance){}

bool DistanceMeasureStrategy::set(StrategySpiData& data){
    data.data[0] = 0;
    data.data[1] = 0;
    data.dataSize -= 2;
    data.data += 2;
    return true;
}

bool DistanceMeasureStrategy::get(StrategySpiData& data){
    *distance = double((data.data[0] << 8) | data.data[1]) / double(0xffff) * 2500.0;
    data.dataSize -= 2;
    data.data += 2;
    return true;
}

unsigned int DistanceMeasureStrategy::getSize(){
    return 2;
}

std::string DistanceMeasureStrategy::getName(){
    return "DistanceMeasureStrategy";
}

}