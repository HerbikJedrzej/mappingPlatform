#include "factory.hh"
#include <MemoryComparator.hh>
#include <Position_Strategy.hh>
#include <Battery_Strategy.hh>
#include <TransmissionKey_Strategy.hh>
#include <DistanceMeasure_Strategy.hh>
#include <Double_Strategy.hh>

Factory::Factory(const std::string& radpberryConfigName, const std::string& _stmConfigName):
    rawConfigData{nullptr, nullptr},
    config(GetRaspberryConfiguration(getRawConfigFile(radpberryConfigName, 0))),
    stmConfig(GetConfiguration(getRawConfigFile(_stmConfigName, 1))),
    stmReadyPin(Drivers::makeGPIO(config->stm_ready_pin(), false)),
    stmConfigName(_stmConfigName){
        log.info(__HEADER__ "Created stm ready pin..");
    }

Factory::~Factory(){
    if(rawConfigData[0])
        delete [] rawConfigData[0];
    if(rawConfigData[1])
        delete [] rawConfigData[1];
}

char* Factory::getRawConfigFile(const std::string& name, unsigned int i){
    std::ifstream infile;
    infile.open(name.c_str(), std::ios::binary | std::ios::in);
    if(!infile.is_open()){
        log.error(__HEADER__ "Config file [" + name + "] cannot be open.");
        throw std::invalid_argument("Config file cannot be open.");
    }
    infile.seekg(0,std::ios::end);
    int length = infile.tellg();
    infile.seekg(0,std::ios::beg);
    rawConfigData[i] = new char[length];
    infile.read(rawConfigData[i], length);
    infile.close();
    return rawConfigData[i];
}

SpiInterfaceMode Factory::flashEEprom(){
    MemoryComparator comparator(stmConfigName,
        config->eeprom_i2c()->bus_name()->str(),
        config->eeprom_i2c()->address(),
        config->write_protect_pin(),
        config->select_i2c_pin());
    return comparator.run();
}

std::unique_ptr<PlatformGroup> Factory::clearAndCreate(){
    log.info(__HEADER__ "Remove old and create new stm platform drivers.");
    std::unique_ptr<PlatformGroup> toReturn(new PlatformGroup(flashEEprom()));
    auto resetPin = Drivers::makeGPIO(config->reset_pin(), true);
    *resetPin = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	*resetPin = true;
	resetPin = Drivers::makeGPIO(0, false);
    std::unique_ptr<ManagerSPI> managerSPI(new ManagerSPI(
        config->spi_bus_name()->str(),
        stmReadyPin,
        getStrategySequence(stmConfig->spi_interface_mode(), *toReturn)
    ));
    toReturn->ledRed = Drivers::makeGPIO(config->red_led(), true);
    toReturn->ledBlue = Drivers::makeGPIO(config->blue_led(), true);
    toReturn->ledGreen = Drivers::makeGPIO(config->green_led(), true);
    toReturn->ledYellow = Drivers::makeGPIO(config->yellow_led(), true);
    managerSPI->init();
    toReturn->setManagerSpi(std::move(managerSPI));
    log.info(__HEADER__ "Stm platform drivers created.");
    return std::move(toReturn);
}

#define NEW_STRATEGY(X) toReturn.push_back(std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::X)

ManagerSPI::StrategySequence Factory::getStrategySequence(const SpiInterfaceMode& id, PlatformGroup& platform){
    ManagerSPI::StrategySequence toReturn{
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(
            stmConfig->spi_recived_key(), stmConfig->spi_transmit_key()))
    };
    switch(id){
        case SpiInterfaceMode::SpiInterfaceMode_NormalMode : {
            platform.batteryCell.push_back(0);
            platform.batteryCell.push_back(0);
            platform.batteryTuneLevel.push_back(0.0);
            platform.batteryTuneLevel.push_back(0.0);
            platform.distance.push_back(0);
            platform.distance.push_back(0);
            platform.distance.push_back(0);
            NEW_STRATEGY(PositionStrategy(
                &platform.currentPosition.x, &platform.currentPosition.y, &platform.currentPosition.angle,
                &platform.expectedPosition.x, &platform.expectedPosition.y, &platform.expectedPosition.angle,
                &platform.applyPosition)));
            NEW_STRATEGY(BatteryStrategy(
                &platform.batteryCell, &platform.batteryTuneLevel)));
            NEW_STRATEGY(DistanceMeasureStrategy(
                &platform.distance[0])));
            NEW_STRATEGY(DistanceMeasureStrategy(
                &platform.distance[1])));
            NEW_STRATEGY(DistanceMeasureStrategy(
                &platform.distance[2])));
            return toReturn;
        }
        case SpiInterfaceMode::SpiInterfaceMode_TuneRotatePID : {
            platform.batteryCell.push_back(0);
            platform.batteryCell.push_back(0);
            platform.batteryTuneLevel.push_back(0.0);
            platform.batteryTuneLevel.push_back(0.0);
            NEW_STRATEGY(DoubleStrategy(
                &platform.setValuePID, &platform.getValuePID)));
            NEW_STRATEGY(BatteryStrategy(
                &platform.batteryCell, &platform.batteryTuneLevel)));
            return toReturn;
        }
        case SpiInterfaceMode::SpiInterfaceMode_TuneMovePID : {
            platform.batteryCell.push_back(0);
            platform.batteryCell.push_back(0);
            platform.batteryTuneLevel.push_back(0.0);
            platform.batteryTuneLevel.push_back(0.0);
            NEW_STRATEGY(DoubleStrategy(
                &platform.setValuePID, &platform.getValuePID)));
            NEW_STRATEGY(BatteryStrategy(
                &platform.batteryCell, &platform.batteryTuneLevel)));
            return toReturn;
        }
        default:
            log.error(__HEADER__ "SPI inteface use empty strategy sequence.");
            return toReturn;
    }
}