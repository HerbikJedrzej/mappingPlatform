#pragma once
#include <vector>
#include <memory>
#include <logSystem.hh>
#include <BusManager.hh>
#include <GPIO_driver.hh>
#include <raspberryPlatform_generated.h>
#include <stmPlatform_generated.h>

struct RobotPosition{
    double x = {0.0};
    double y = {0.0};
    double angle = {0.0};
};

struct PlatformGroup{
    RobotPosition expectedPosition;
    RobotPosition currentPosition;
    bool applyPosition = false;
    std::shared_ptr<Drivers::GPIOifc> ledGreen  = {nullptr};
    std::shared_ptr<Drivers::GPIOifc> ledBlue   = {nullptr};
    std::shared_ptr<Drivers::GPIOifc> ledYellow = {nullptr};
    std::shared_ptr<Drivers::GPIOifc> ledRed    = {nullptr};
    std::vector<double> batteryCell;
    std::vector<double> batteryTuneLevel;
    std::vector<double> distance;
    double setValuePID = {0.0}, getValuePID = {0.0};
    
    PlatformGroup(const SpiInterfaceMode& _mode):
        mode(_mode){}
    PlatformGroup():
        mode(SpiInterfaceMode::SpiInterfaceMode_NormalMode){}
    SpiInterfaceMode getMode(){
        return mode;
    }
    void setManagerSpi(std::unique_ptr<ManagerSPI>&& manager){
        managerSPI = std::move(manager);
    }
    bool getAndClearCycleFlag(){
        const bool toReturn = managerSPI->getFlag();
        managerSPI->clearFlag();
        return toReturn;
    }
private:
    SpiInterfaceMode mode;
    std::unique_ptr<ManagerSPI> managerSPI = {nullptr};
};

class Factory{
public:
    Factory(const std::string& radpberryConfigName, const std::string& _stmConfigName);
    ~Factory();
    std::unique_ptr<PlatformGroup> clearAndCreate();
private:
    ManagerSPI::StrategySequence getStrategySequence(const SpiInterfaceMode& id, PlatformGroup& platformGroup);
    SpiInterfaceMode flashEEprom();
    char* getRawConfigFile(const std::string& name, unsigned int i);
    Log log;
    char* rawConfigData[2];
    const RaspberryConfiguration *config;
    const Configuration *stmConfig;
    std::shared_ptr<Drivers::GPIOifc> stmReadyPin;
    const std::string stmConfigName;
};