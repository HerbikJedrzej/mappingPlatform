#include <gtest/gtest.h>
#include <Double_Strategy.hh>
#include <AllDataUsed_Strategy.hh>
#include <TransmissionKey_Strategy.hh>
#include <memory>
#include <vector>

TEST(Double_Strategy, get_set){TEST_LOG_HEADER
    constexpr unsigned int size = 9;
    uint8_t data[size];
    double setVal = -125.57;
    double getVal = 0.0;
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x68, 0x68)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::DoubleStrategy(&setVal, &getVal)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(nullptr, &data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(size, strategySize);
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->set(strategyData));
    strategyData = {data, size};
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->get(strategyData));
    EXPECT_DOUBLE_EQ(setVal, getVal);
    for(unsigned int i = 1; i < 9; i++)
        EXPECT_NE(data[i], 0) << "i = " << i << std::endl;
}

TEST(Double_Strategy, getSize){TEST_LOG_HEADER
    double X;
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::DoubleStrategy(&X, &X));
    EXPECT_EQ(strategy->getSize(), 8);
}
