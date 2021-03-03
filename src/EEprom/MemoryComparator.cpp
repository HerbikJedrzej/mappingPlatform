#include "MemoryComparator.hh"

MemoryComparator::MemoryComparator(const std::string& name, const std::string& i2cPort, const uint8_t devAddr, const uint8_t writeProtectPin, const uint8_t selectPin):
    jsonProvider(name),
    writeProtect(nullptr),
    select(nullptr),
    i2c(nullptr){
        writeProtect = Drivers::makeGPIO(writeProtectPin, true);
		*writeProtect = true;
        log.info(__HEADER__ "Created memory write protect pin.");
        select = Drivers::makeGPIO(selectPin, true);
		*select = true;
        log.info(__HEADER__ "Created select pin.");
        i2c = Drivers::makeI2C(i2cPort, devAddr); // 0xA0
        log.info(__HEADER__ "Created I2C driver.");
    }

MemoryComparator::~MemoryComparator(){
	*writeProtect = true;
    log.info(__HEADER__ "Memory write protection switched on.");
	*select = false;
    log.info(__HEADER__ "EEprom I2C bus switched to STM.");
}

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
    printValue("Old", dataFromEEprom);
    i2c->Write(addr, dataFromJson);
    printValue("New", dataFromJson);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    *writeProtect = true;
    return false;
}

void MemoryComparator::printValue(const std::string& prefix, const std::vector<uint8_t>& data){
    std::string value;
    for(const auto& elem : data)
        value += " " + std::to_string(elem);
    log.info(prefix + " value: " + value);
}

SpiInterfaceMode MemoryComparator::run(){
    bool meoryNotChanged = true;

    meoryNotChanged &= process          (0x0000, "EEprom key"                  , jsonProvider.eeprom_key());
    meoryNotChanged &= process          (0x000a, "SPI transmit key"            , jsonProvider.spi_transmit_key());
    meoryNotChanged &= process          (0x000b, "SPI recived key"             , jsonProvider.spi_recived_key());
    meoryNotChanged &= process          (0x000c, "SPI interface mode"          , jsonProvider.spi_interface_mode());
    meoryNotChanged &= process<double>  (0x0100, "Stright movement P"          , jsonProvider.stright_movement__p());
    meoryNotChanged &= process<double>  (0x0108, "Stright movement I"          , jsonProvider.stright_movement__i());
    meoryNotChanged &= process<double>  (0x0110, "Stright movement D"          , jsonProvider.stright_movement__d());
    meoryNotChanged &= process<double>  (0x0118, "Stright movement anti windup", jsonProvider.stright_movement__anti_windup());
    meoryNotChanged &= process<double>  (0x0120, "Rotate movement P"           , jsonProvider.rotate_movement__p());
    meoryNotChanged &= process<double>  (0x0128, "Rotate movement I"           , jsonProvider.rotate_movement__i());
    meoryNotChanged &= process<double>  (0x0130, "Rotate movement D"           , jsonProvider.rotate_movement__d());
    meoryNotChanged &= process<double>  (0x0138, "Rotate movement anti windup" , jsonProvider.rotate_movement__anti_windup());
    meoryNotChanged &= process<uint8_t> (0x0203, "Number of battery cells"     , jsonProvider.battery_cells_size());
    meoryNotChanged &= process<double>  (0x0204, "Max measured battery voltage", jsonProvider.max_measured_voltage());
    meoryNotChanged &= process<uint32_t>(0x020c, "Max battery ADC word"        , jsonProvider.battery_adc_max_word());
    for(unsigned int i = 0; i < jsonProvider.battery_cells_size()[0]; i++){
        meoryNotChanged &= process<uint32_t>(0x0210 + i * 8, "Upper resistors " + std::to_string(i), jsonProvider.upper_resistor_ohm(i));
        meoryNotChanged &= process<uint32_t>(0x0214 + i * 8, "Lower resistors " + std::to_string(i), jsonProvider.lower_resistor_ohm(i));
    }

    log.info(__HEADER__ ((meoryNotChanged)? "Memory check done. Nothing was changed." : "Memory check done."));
    return jsonProvider.spiInterfaceMode();
}
