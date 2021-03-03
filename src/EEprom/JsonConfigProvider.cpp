#include "JsonConfigProvider.hh"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

JsonConfigProvider::JsonConfigProvider(const std::string& name):
    rawConfigData(nullptr),
    config(
        [&]()->const Configuration*{
            std::ifstream infile;
            infile.open(name.c_str(), std::ios::binary | std::ios::in);
            if(!infile.is_open()){
                log.error(__HEADER__ "Config file [" + name + "] cannot be open.");
                throw std::invalid_argument("Config file cannot be open.");
            }
            infile.seekg(0,std::ios::end);
            int length = infile.tellg();
            infile.seekg(0,std::ios::beg);
            rawConfigData = new char[length];
            infile.read(rawConfigData, length);
            infile.close();
            return GetConfiguration(rawConfigData);
        }()
    ){}

JsonConfigProvider::~JsonConfigProvider(){
    if(rawConfigData)
        delete [] rawConfigData;
}

    SpiInterfaceMode JsonConfigProvider::spiInterfaceMode(){
        return config->spi_interface_mode();
    }

    std::vector<uint8_t> JsonConfigProvider::eeprom_key(){
        return std::vector<uint8_t>{0x15, 0x26, 0x37, 0x48, 0x59, 0x6a, 0x7b, 0x8c, 0x9d, 0xae};
    }
    std::vector<uint8_t> JsonConfigProvider::spi_transmit_key(){
        return get(config->spi_transmit_key());
    }
    std::vector<uint8_t> JsonConfigProvider::spi_recived_key(){
        return get(config->spi_recived_key());
    }
    std::vector<uint8_t> JsonConfigProvider::spi_interface_mode(){
        return get(uint8_t(config->spi_interface_mode()));
    }
    std::vector<uint8_t> JsonConfigProvider::stright_movement__p(){
        return get(config->stright_movement()->p());
    }
    std::vector<uint8_t> JsonConfigProvider::stright_movement__i(){
        return get(config->stright_movement()->i());
    }
    std::vector<uint8_t> JsonConfigProvider::stright_movement__d(){
        return get(config->stright_movement()->d());
    }
    std::vector<uint8_t> JsonConfigProvider::stright_movement__anti_windup(){
        return get(config->stright_movement()->anti_windup());
    }
    std::vector<uint8_t> JsonConfigProvider::rotate_movement__p(){
        return get(config->rotate_movement()->p());
    }
    std::vector<uint8_t> JsonConfigProvider::rotate_movement__i(){
        return get(config->rotate_movement()->i());
    }
    std::vector<uint8_t> JsonConfigProvider::rotate_movement__d(){
        return get(config->rotate_movement()->d());
    }
    std::vector<uint8_t> JsonConfigProvider::rotate_movement__anti_windup(){
        return get(config->rotate_movement()->anti_windup());
    }
    std::vector<uint8_t> JsonConfigProvider::battery_cells_size(){
        if(!config->battery_cells())
            return std::vector<uint8_t>{0};
        return std::vector<uint8_t>{uint8_t(config->battery_cells()->size())};
    }
    std::vector<uint8_t> JsonConfigProvider::max_measured_voltage(){
        return get(config->max_measured_voltage());
    }
    std::vector<uint8_t> JsonConfigProvider::battery_adc_max_word(){
        return get(config->battery_adc_max_word());
    }
    std::vector<uint8_t> JsonConfigProvider::upper_resistor_ohm(unsigned int cell){
        return get((*config->battery_cells())[cell]->upper_resistor_ohm());
    }
    std::vector<uint8_t> JsonConfigProvider::lower_resistor_ohm(unsigned int cell){
        return get((*config->battery_cells())[cell]->lower_resistor_ohm());
    }
