#include <gtest/gtest.h>
#include <Position_Strategy.hh>
#include <AllDataUsed_Strategy.hh>
#include <TransmissionKey_Strategy.hh>
#include <memory>
#include <vector>

namespace {

double calculatePosition(uint8_t data[4]){
    return double(int32_t((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])) / 1000;
}

std::vector<uint8_t> calculatePosition(double X, double Y, double rotate, bool apply = true){
    int32_t rawX = X * 1000;
    int32_t rawY = Y * 1000;
    int32_t rawR = rotate * double(0x8000) / 360.0;
    return std::vector<uint8_t>{
        uint8_t((rawX >> 24) & 0xff),
        uint8_t((rawX >> 16) & 0xff),
        uint8_t((rawX >> 8) & 0xff),
        uint8_t(rawX & 0xff),
        uint8_t((rawY >> 24) & 0xff),
        uint8_t((rawY >> 16) & 0xff),
        uint8_t((rawY >> 8) & 0xff),
        uint8_t(rawY & 0xff),
        uint8_t((apply)? 0x80 | ((rawR >> 8) & 0x7f) : (rawR >> 8) & 0x7f),
        uint8_t(rawR & 0xff)
    };
}

double calculateRotation(uint8_t data[2]){
    return double(uint16_t(((data[0] & 0x7f) << 8) | data[1])) * 360.0 / double(0x8000);
}

}

TEST(Position_Strategy, get){TEST_LOG_HEADER
    constexpr unsigned int size = 11;
    uint8_t data[size];
    double X;
    double Y;
    double R;
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x68, 0xab)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::PositionStrategy(
        &X, &Y, &R, &X, &X, &X
    )));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::AllDataUsedStrategy(nullptr, &data[size - 1])));
    unsigned int strategySize = 0;
    for(const auto& elem : strategy)
        strategySize += elem->getSize();
    ASSERT_EQ(size, strategySize);
    data[0] = 0x68;
    unsigned int i = 1;
    for(const auto& elem : calculatePosition(12.54, -24.56, 25.6))
        data[i++] = elem;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->get(strategyData));
    EXPECT_EQ(strategyData.data, data + size);
    EXPECT_EQ(strategyData.dataSize, 0);
    EXPECT_DOUBLE_EQ(X, 12.54);
    EXPECT_DOUBLE_EQ(Y, -24.56);
    EXPECT_NEAR(R, 25.6, 0.01);
}

TEST(Position_Strategy, set){TEST_LOG_HEADER
    constexpr unsigned int size = 11;
    uint8_t data[size];
    double X = 0.35;
    double Y = 2.45;
    double R = 96.8;
    bool apply = false;
    StrategySpi::StrategySpiData strategyData{data, size};
    std::vector<std::unique_ptr<StrategySpi::StrategySpiIfc>> strategy;
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x68, 0xab)));
    strategy.push_back(std::unique_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::PositionStrategy(
        &X, &X, &X, &X, &Y, &R, &apply
    )));
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
    EXPECT_DOUBLE_EQ(X, calculatePosition(&data[1]));
    EXPECT_DOUBLE_EQ(Y, calculatePosition(&data[5]));
    EXPECT_NEAR(R, calculateRotation(&data[9]), 0.02);

    X = 25.0;
    Y = 2.12;
    R = -254.25;
    apply = true;
    strategyData.data = data;
    strategyData.dataSize = size;
    for(const auto& elem : strategy)
        ASSERT_TRUE(elem->set(strategyData));
    EXPECT_EQ(strategyData.data, data + size);
    EXPECT_EQ(strategyData.dataSize, 0);
    auto dataToCeck = calculatePosition(X, Y, R, apply);
    ASSERT_EQ(data[0], 0xab);
    EXPECT_DOUBLE_EQ(X, calculatePosition(&data[1]));
    EXPECT_DOUBLE_EQ(Y, calculatePosition(&data[5]));
    EXPECT_EQ(data[9], 0x80);
    EXPECT_EQ(data[10], 0);
}

TEST(Position_Strategy, getSize){TEST_LOG_HEADER
    double X;
    std::shared_ptr<StrategySpi::StrategySpiIfc> strategy(new StrategySpi::PositionStrategy(
        &X, &X, &X, &X, &X, &X
    ));
    EXPECT_EQ(strategy->getSize(), 10);
}
