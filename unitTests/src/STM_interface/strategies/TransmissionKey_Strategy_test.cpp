#include <gtest/gtest.h>
#include <TransmissionKey_Strategy.hh>
#include <AllDataUsed_Strategy.hh>
#include <memory>
#include <vector>

TEST(TransmissionKey_Strategy, get){TEST_LOG_HEADER
    constexpr unsigned int size = 1;
    uint8_t data[size];
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x25, 0x46)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(nullptr, &data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(1, strategySize);
    data[0] = 0x24;
    EXPECT_FALSE(strategy[0]->get(strategyData));
    EXPECT_EQ(strategyData.data, data);
    EXPECT_EQ(strategyData.dataSize, 1);
    strategyData.data = data;
    strategyData.dataSize = size;
    data[0] = 0x25;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->get(strategyData));
    EXPECT_EQ(strategyData.data, data + 1);
    EXPECT_EQ(strategyData.dataSize, 0);
}

TEST(TransmissionKey_Strategy, set){TEST_LOG_HEADER
    constexpr unsigned int size = 1;
    uint8_t data[size];
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x20, 0x56)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(&data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(1, strategySize);
    data[0] = 0;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->set(strategyData));
    EXPECT_EQ(strategyData.data, data + 1);
    EXPECT_EQ(strategyData.dataSize, 0);
    EXPECT_EQ(data[0], 0x56);
}

TEST(TransmissionKey_Strategy, getSize){TEST_LOG_HEADER
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::TransmissionKeyStrategy(0x25, 0x25));
    EXPECT_EQ(strategy->getSize(), 1);
}
