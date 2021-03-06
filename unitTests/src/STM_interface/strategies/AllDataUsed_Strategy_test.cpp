#include <gtest/gtest.h>
#include <AllDataUsed_Strategy.hh>
#include <memory>

TEST(AllDataUsed_Strategy, get){
    constexpr unsigned int size = 10;
    uint8_t data[size + 1];
    StrategySpi::StrategySpiData strategyData{data, size};
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::AllDataUsedStrategy());
    EXPECT_FALSE(strategy->get(strategyData));
    strategyData.dataSize = 1;
    EXPECT_FALSE(strategy->get(strategyData));
    strategyData.dataSize = 0;
    EXPECT_TRUE(strategy->get(strategyData));
}

TEST(AllDataUsed_Strategy, getWithLastAdressReference){
    constexpr unsigned int size = 2;
    uint8_t data[size];
    StrategySpi::StrategySpiData strategyData{data, size};
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::AllDataUsedStrategy(nullptr, &data[size - 1]));
    EXPECT_FALSE(strategy->get(strategyData));
    strategyData.dataSize = 0;
    EXPECT_FALSE(strategy->get(strategyData));
    strategyData.data = data + 2;
    EXPECT_TRUE(strategy->get(strategyData));
    strategyData.dataSize = 1;
    EXPECT_FALSE(strategy->get(strategyData));
    strategyData.dataSize = 0;
    EXPECT_TRUE(strategy->get(strategyData));
    strategyData.data = data + 1;
    EXPECT_FALSE(strategy->get(strategyData));
}

TEST(AllDataUsed_Strategy, setWithLastAdressReference){
    constexpr unsigned int size = 2;
    uint8_t data[size];
    StrategySpi::StrategySpiData strategyData{data, size};
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::AllDataUsedStrategy(&data[size - 1]));
    EXPECT_FALSE(strategy->set(strategyData));
    strategyData.dataSize = 0;
    EXPECT_FALSE(strategy->set(strategyData));
    strategyData.data = data + 2;
    EXPECT_TRUE(strategy->set(strategyData));
    strategyData.dataSize = 1;
    EXPECT_FALSE(strategy->set(strategyData));
    strategyData.dataSize = 0;
    EXPECT_TRUE(strategy->set(strategyData));
    strategyData.data = data + 1;
    EXPECT_FALSE(strategy->set(strategyData));
}

TEST(AllDataUsed_Strategy, getSize){
    constexpr unsigned int size = 2;
    uint8_t data[size];
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy1(new StrategySpi::AllDataUsedStrategy(&data[size - 1]));
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy2(new StrategySpi::AllDataUsedStrategy());
    EXPECT_EQ(strategy1->getSize(), 0);
    EXPECT_EQ(strategy2->getSize(), 0);
}
