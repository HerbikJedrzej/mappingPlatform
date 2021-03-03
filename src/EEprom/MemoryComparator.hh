#pragma once
#include <logSystem.hh>
#include "JsonConfigProvider.hh"
#include <I2C_driver.hh>
#include <GPIO_driver.hh>
#include <chrono>
#include <thread>

class MemoryComparator{
public:
    MemoryComparator(const std::string& name, const std::string& i2cPort, const uint8_t devAddr, const uint8_t writeProtectPin, const uint8_t selectPin);
    virtual ~MemoryComparator();
    virtual SpiInterfaceMode run();
protected:
    Log log;
    JsonConfigProvider jsonProvider;
    bool process(uint16_t addr, const std::string& name, const std::vector<uint8_t>& dataFromJson);
    template <typename T>
    bool process(uint16_t addr, const std::string& name, const std::vector<uint8_t>& dataFromJson);
private:
    std::shared_ptr<Drivers::GPIOifc> writeProtect;
    std::shared_ptr<Drivers::GPIOifc> select;
    std::shared_ptr<Drivers::I2Cifc> i2c;
    void printValue(const std::string& prefix, const std::vector<uint8_t>& data);
    template <typename T>
    void printValue(const std::string& prefix, const std::vector<uint8_t>& data);
    template <typename T>
    union Converter{
        uint8_t array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        T val;
    };
};

template <typename T>
bool MemoryComparator::process(uint16_t addr, const std::string& name, const std::vector<uint8_t>& dataFromJson){
    auto dataFromEEprom = i2c->Read(addr, dataFromJson.size());
    log.debug("Memory check: " + name);
    if(dataFromEEprom.size() != dataFromJson.size()){
        log.error(__HEADER__ "Data size from json ["
            + std::to_string(dataFromJson.size())
            + "] is differen than data size from EEprom ["
            + std::to_string(dataFromEEprom.size()) + "].");
        return true;
    }
    if(dataFromEEprom == dataFromJson)
        return true;
    *writeProtect = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    log.warning("Differen values of '" + name + "' between json and EEprom.");
    printValue<T>("Old", dataFromEEprom);
    i2c->Write(addr, dataFromJson);
    printValue<T>("New", dataFromJson);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    *writeProtect = true;
    return false;
}

template <typename T>
void MemoryComparator::printValue(const std::string& prefix, const std::vector<uint8_t>& data){
    Converter<T> converter;
    printValue(prefix, data);
    if(data.size() > 8){
        log.error(__HEADER__ "Too many bytes [" + std::to_string(data.size()) + "] needed to print conversion.");
        return;
    }
    for(uint8_t i = 0; i < data.size(); i++)
        converter.array[i] = data[i];
    log.info(prefix + " original type value: " + std::to_string(converter.val));
}
