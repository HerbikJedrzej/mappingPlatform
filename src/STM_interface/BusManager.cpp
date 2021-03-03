#include "BusManager.hh"
#include <chrono>
#include <limits>
#include "strategies/AllDataUsed_Strategy.hh"

ManagerSPI::ManagerSPI(const std::string& spiName, const std::shared_ptr<Drivers::GPIOifc>& stmReadyPinNr, const StrategySequence& _strategies):
    strategies(_strategies),
    timerFlag(false),
    externalCycleFlag(false),
    stmReady(stmReadyPinNr),
    spi(Drivers::makeSPI(spiName)),
    runFlag(true),
    dataSize([&]()->unsigned int{
        unsigned int size = 0;
        for(unsigned int i = 0; i < strategies.size(); i++)
            if(strategies[i])
                size += strategies[i]->getSize();
            else
                throw std::invalid_argument("Nullptr at strategy pointer nr. " + std::to_string(i));
        return size;
    }()),
    dataSend(new uint8_t[dataSize]),
    dataRecived(new uint8_t[dataSize]),
    watcher(nullptr),
    timer(nullptr){
        if(dataSize == 0)
            log.warning(__HEADER__ "ManagerSPI created. SPI protocal size = 0.");
        else
            log.info(__HEADER__ "ManagerSPI created. SPI protocal size = " + std::to_string(dataSize));
        strategies.push_back(std::shared_ptr<StrategySpi::StrategySpiIfc>(
            new StrategySpi::AllDataUsedStrategy(&dataSend[dataSize - 1], &dataRecived[dataSize - 1])
        ));
    }

ManagerSPI::ManagerSPI(const std::string& spiName, const unsigned int& stmReadyPinNr, const StrategySequence& _strategies):
    ManagerSPI(spiName, Drivers::makeGPIO(stmReadyPinNr, false), _strategies){}

bool ManagerSPI::getFlag(){
    return externalCycleFlag;
}

void ManagerSPI::clearFlag(){
    externalCycleFlag = false;
}

void ManagerSPI::init(){
    watcher.reset(new std::thread(&ManagerSPI::watchLoop, this));
    log.info(__HEADER__ "SPI bus watcher is running.");
    timer.reset(new std::thread(&ManagerSPI::timeLoop, this));
    log.info(__HEADER__ "SPI bus timer is running.");
}

ManagerSPI::~ManagerSPI(){
    runFlag = false;
    if(watcher)
        watcher->join();
    log.info(__HEADER__ "SPI bus watcher is stoped.");
    if(timer)
        timer->join();
    log.info(__HEADER__ "SPI bus timer is stoped.");
}

void ManagerSPI::watchLoop(){
    while(runFlag)
        if(timerFlag){
            timerFlag = false;
            cycle();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
}

void ManagerSPI::timeLoop(){
    while(runFlag){
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        timerFlag = true;
    }
}

void ManagerSPI::cycle(){
    if(*stmReady){
        parseOutgoingData();
        spi->transfer(dataSend, dataRecived, dataSize);
        parseIncomingData();
        clearIncomingData();
    }else
        log.debug(__HEADER__ "STM not ready for SPI transmission.");
    externalCycleFlag = true;
}

void ManagerSPI::clearIncomingData(){
    for(uint8_t i = 0; i < dataSize; i++)
        dataRecived[i] = 0;
}

void ManagerSPI::parseOutgoingData(){
    StrategySpi::StrategySpiData strategyData{dataSend, dataSize};
    for(const auto& strategy : strategies)
        if(!strategy->set(strategyData)){
            log.warning(__HEADER__ "Method set failed at strategy: " + strategy->getName());
            return;
        }
    log.debug(__HEADER__ "Output data are parsed.");
}

void ManagerSPI::parseIncomingData(){
    StrategySpi::StrategySpiData strategyData{dataRecived, dataSize};
    for(const auto& strategy : strategies)
        if(!strategy->get(strategyData)){
            log.warning(__HEADER__ "Method get failed at strategy: " + strategy->getName());
            return;
        }
    log.debug(__HEADER__ "Input data are parsed.");
}
