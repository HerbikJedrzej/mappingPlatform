#include <gtest/gtest.h>
#include <DistanceMeasure_Strategy.hh>
#include <AllDataUsed_Strategy.hh>
#include <TransmissionKey_Strategy.hh>
#include <memory>
#include <vector>

TEST(DistanceMeasure_Strategy, get){TEST_LOG_HEADER
    constexpr unsigned int size = 3;
    uint8_t data[size];
    double distance;
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x68, 0xab)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::DistanceMeasureStrategy(&distance)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(nullptr, &data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(size, strategySize);
    data[0] = 0x68;
    data[1] = 0x00;
    data[2] = 0x00;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->get(strategyData));
    EXPECT_EQ(strategyData.data, data + size);
    EXPECT_EQ(strategyData.dataSize, 0);
    EXPECT_DOUBLE_EQ(distance, 0.0);

    strategyData.data = data;
    strategyData.dataSize = size;
    data[0] = 0x68;
    data[1] = 0xff;
    data[2] = 0xff;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->get(strategyData));
    EXPECT_EQ(strategyData.data, data + size);
    EXPECT_EQ(strategyData.dataSize, 0);
    EXPECT_DOUBLE_EQ(distance, 2500.0);

    strategyData.data = data;
    strategyData.dataSize = size;
    data[0] = 0x68;
    data[1] = 0xff / 2;
    data[2] = 0xff;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->get(strategyData));
    EXPECT_EQ(strategyData.data, data + size);
    EXPECT_EQ(strategyData.dataSize, 0);
    EXPECT_NEAR(distance, 2500.0 / 2, 0.02);
}

TEST(DistanceMeasure_Strategy, set){TEST_LOG_HEADER
    constexpr unsigned int size = 3;
    uint8_t data[size];
    double distance;
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x68, 0xab)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::DistanceMeasureStrategy(&distance)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(&data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(size, strategySize);
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->set(strategyData));
    EXPECT_EQ(strategyData.data, data + size);
    EXPECT_EQ(strategyData.dataSize, 0);
    ASSERT_EQ(data[0], 0xab);
}

TEST(DistanceMeasure_Strategy, getSize){TEST_LOG_HEADER
    double X;
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::DistanceMeasureStrategy(&X));
    EXPECT_EQ(strategy->getSize(), 2);
}
