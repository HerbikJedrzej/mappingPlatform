#pragma once
#include <GPIO_driver.hh>
#include <SPI_driver.hh>
#include <thread>
#include <memory>
#include <logSystem.hh>
#include <vector>
#include "strategies/StrategySPI_ifc.hh"

class ManagerSPI{
public:
    using StrategySequence = std::vector<std::shared_ptr<StrategySpi::StrategySpiIfc> >;
    ManagerSPI(const std::string& spiName, const unsigned int& stmReadyPinNr, const StrategySequence& _strategies);
    ManagerSPI(const std::string& spiName, const std::shared_ptr<Drivers::GPIOifc>& stmReadyPinNr, const StrategySequence& _strategies);
    ~ManagerSPI();
    void init();
    bool getFlag();
    void clearFlag();
protected:
    virtual void watchLoop();
    void cycle();
    Log log;
private:
    StrategySequence strategies;
    void timeLoop();
    void parseOutgoingData();
    void parseIncomingData();
    void clearIncomingData();
    bool timerFlag;
    bool externalCycleFlag;
    std::shared_ptr<Drivers::GPIOifc> stmReady;
    std::shared_ptr<Drivers::SPIifc> spi;
    bool runFlag;
    const unsigned int dataSize;
    uint8_t* dataSend;
    uint8_t* dataRecived;
    std::shared_ptr<std::thread> watcher;
    std::shared_ptr<std::thread> timer;
};
