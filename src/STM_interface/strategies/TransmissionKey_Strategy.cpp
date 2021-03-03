#include "TransmissionKey_Strategy.hh"

namespace StrategySpi{

TransmissionKeyStrategy::TransmissionKeyStrategy(const uint8_t& keyRecived, const uint8_t& keyTransmitted):
    keyR(keyRecived),
    keyT(keyTransmitted){}

bool TransmissionKeyStrategy::set(StrategySpiData& data){
    data.data[0] = keyT;
    data.dataSize--;
    data.data++;
    return true;
}

bool TransmissionKeyStrategy::get(StrategySpiData& data){
    if(data.data[0] != keyR){
        log.warning(__HEADER__ "Wrong key. Expected: " + std::to_string(keyR) + " recived: " + std::to_string(data.data[0]));
        return false;
    }
    data.dataSize--;
    data.data++;
    return true;
}

unsigned int TransmissionKeyStrategy::getSize(){
    return 1;
}

std::string TransmissionKeyStrategy::getName(){
    return "TransmissionKeyStrategy";
}

}