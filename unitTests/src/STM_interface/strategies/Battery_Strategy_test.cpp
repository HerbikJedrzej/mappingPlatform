#include <gtest/gtest.h>
#include <Battery_Strategy.hh>
#include <AllDataUsed_Strategy.hh>
#include <TransmissionKey_Strategy.hh>
#include <memory>
#include <vector>

TEST(Battery_Strategy, set_get){TEST_LOG_HEADER
    constexpr unsigned int size = 3;
    uint8_t data[size];
    std::vector<double> setVal = { 0.0, 0.0, 0.0,  0.0,  0.0, 0.0, 0.0,  0.0};
    std::vector<double> getVal = {11.5, 5.7, 2.6, 16.8, 23.2, 7.4, 8.6, 28.9};
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x68, 0x68)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::BatteryStrategy(&setVal, &getVal)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(nullptr, &data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(size, strategySize);

    for(uint16_t j = 0; j < getVal.size(); j++){
        strategyData = {data, size};
        for(const auto& elem : strategy)
            ASSERT_TRUE(elem->set(strategyData));
        strategyData = {data, size};
        for(const auto& elem : strategy)
            ASSERT_TRUE(elem->get(strategyData));
        for(uint16_t i = 0; i <= j; i++)
            EXPECT_DOUBLE_EQ(setVal[i], getVal[i]) << "j = " << j << " i = " << i;
        for(uint16_t i = j + 1; i < getVal.size(); i++)
            EXPECT_NE(setVal[i], getVal[i]) << "j = " << j << "i = " << i;
        for(unsigned int i = 1; i < size; i++)
            EXPECT_NE(data[i], 0) << "i = " << i << std::endl;
    }
}

TEST(Battery_Strategy, getSize){TEST_LOG_HEADER
    std::vector<double> X;
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::BatteryStrategy(&X, &X));
    EXPECT_EQ(strategy->getSize(), 2);
}
