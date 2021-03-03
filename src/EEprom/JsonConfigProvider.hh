#pragma once
#include <stmPlatform_generated.h>
#include <logSystem.hh>

class JsonConfigProvider{
public:
    JsonConfigProvider(const std::string& name);
    ~JsonConfigProvider();
    SpiInterfaceMode spiInterfaceMode();
    std::vector<uint8_t> eeprom_key();
    std::vector<uint8_t> spi_transmit_key();
    std::vector<uint8_t> spi_recived_key();
    std::vector<uint8_t> spi_interface_mode();
    std::vector<uint8_t> stright_movement__p();
    std::vector<uint8_t> stright_movement__i();
    std::vector<uint8_t> stright_movement__d();
    std::vector<uint8_t> stright_movement__anti_windup();
    std::vector<uint8_t> rotate_movement__p();
    std::vector<uint8_t> rotate_movement__i();
    std::vector<uint8_t> rotate_movement__d();
    std::vector<uint8_t> rotate_movement__anti_windup();
    std::vector<uint8_t> battery_cells_size();
    std::vector<uint8_t> max_measured_voltage();
    std::vector<uint8_t> battery_adc_max_word();
    std::vector<uint8_t> upper_resistor_ohm(unsigned int cell);
    std::vector<uint8_t> lower_resistor_ohm(unsigned int cell);
private:
    Log log;
    char* rawConfigData;
    const Configuration *config;
    template <typename Type>
    union ConvertDouble{
        Type value;
        uint8_t array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    };
    template <typename Type>
    std::vector<uint8_t> get(const Type& elem){
        const unsigned int size = sizeof(Type);
        if(size > 8)
            throw std::invalid_argument("Unsupported type convertion which need " + std::to_string(size) + " bytes.");
        ConvertDouble<Type> converter;
        std::vector<uint8_t> toReturn;
        converter.value = elem;
        for(uint8_t i = 0; i < size; i++ ){
            toReturn.push_back(converter.array[i]);
        }
        return std::move(toReturn);
    }
};