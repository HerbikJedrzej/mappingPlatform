#include "AllDataUsed_Strategy.hh"

namespace StrategySpi{

AllDataUsedStrategy::AllDataUsedStrategy(const uint8_t* lastTransmittedElement, const uint8_t* lastRecivedElement):
    lastTxElem(lastTransmittedElement),
    lastRxElem(lastRecivedElement){}

bool AllDataUsedStrategy::set(StrategySpiData& data){
    if(lastTxElem)
        if(lastTxElem + 1 != data.data)
            return false;
    if(0 != data.dataSize)
        return false;
    return true;
}

bool AllDataUsedStrategy::get(StrategySpiData& data){
    if(lastRxElem)
        if(lastRxElem + 1 != data.data)
            return false;
    if(0 != data.dataSize)
        return false;
    return true;
}

unsigned int AllDataUsedStrategy::getSize(){
    return 0;
}

std::string AllDataUsedStrategy::getName(){
    return "AllDataUsedStrategy";
}

}