#include "Double_Strategy.hh"

namespace StrategySpi{

DoubleStrategy::DoubleStrategy(
        double* _setValue,
        double* _getValue
    ):
    setValue(_setValue),
    getValue(_getValue){}

bool DoubleStrategy::set(StrategySpiData& data){
    converter.val = *setValue;
    for(unsigned int i = 0; i < 8; i++)
        data.data[i] = converter.array[i];
    data.dataSize -= 8;
    data.data += 8;
    return true;
}

bool DoubleStrategy::get(StrategySpiData& data){
    for(unsigned int i = 0; i < 8; i++)
        converter.array[i] = data.data[i];
    *getValue = converter.val;
    data.dataSize -= 8;
    data.data += 8;
    return true;
}

unsigned int DoubleStrategy::getSize(){
    return 8;
}

std::string DoubleStrategy::getName(){
    return "DoubleStrategy";
}

}