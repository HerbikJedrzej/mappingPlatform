#include <gtest/gtest.h>
#include <BusManager.hh>
#include <SPI_driver_Mock.hh>
#include <GPIO_driver_Mock.hh>
#include <AllDataUsed_Strategy.hh>
#include <Position_Strategy.hh>
#include <Battery_Strategy.hh>
#include <TransmissionKey_Strategy.hh>
#include <DistanceMeasure_Strategy.hh>

namespace {
    class ManagerSPItestHelper : public ManagerSPI{
    public:
        ManagerSPItestHelper(const StrategySequence& stategies):
            ManagerSPI("/dev/spidev0.0", 22, stategies){}
        void watchLoop() final{
            cycle();
        }
    };
}

using namespace StrategySpi;

TEST(ManagerSPI, cycle){TEST_LOG_HEADER
    auto gpio = Drivers::createDriverGPIO({22}).at(22);
    auto spi = Drivers::createDriverSPI();
    gpio->setPinReading({
        false,
        true,
        true
    });
    spi->setReciveBuffor({
        {0x25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    });
    double actPosX = 0;
    double actPosY = 0;
    double actRot = 0;
    double expPosX = 10.0;
    double expPosY = -2.5;
    double expRot = 180.0;
    bool applyPosition = false;
    std::vector<double> percentBattery = {0.0, 0.0};
    std::vector<double> tuneVoltage = {0.0, 0.0};
    double distance[3] = {0, 0, 0};
    ManagerSPItestHelper manager(ManagerSPI::StrategySequence{
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x25, 0xab)),
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::PositionStrategy(&actPosX, &actPosY, &actRot, &expPosX, &expPosY, &expRot, &applyPosition)),
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::BatteryStrategy(&percentBattery, &tuneVoltage)),
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::DistanceMeasureStrategy(&distance[0])),
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::DistanceMeasureStrategy(&distance[1])),
        std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::DistanceMeasureStrategy(&distance[2])),
    });
    manager.watchLoop();
    manager.watchLoop();
    auto sentSpiData = spi->getSendLine();
    ASSERT_EQ(sentSpiData.size(), 19);
    EXPECT_EQ(sentSpiData[0], 0xab);

    EXPECT_NE(actPosX, expPosX);
    EXPECT_NE(actPosY, expPosY);
    EXPECT_NE(actRot, expRot);
    sentSpiData[0] = 0x25;
    spi->setReciveBuffor({
        sentSpiData
    });
    EXPECT_NE(actPosX, expPosX);
    EXPECT_NE(actPosY, expPosY);
    EXPECT_NE(actRot, expRot);

    manager.watchLoop();

    EXPECT_DOUBLE_EQ(expPosX, 10.0);
    EXPECT_DOUBLE_EQ(expPosY, -2.5);
    EXPECT_DOUBLE_EQ(expRot, 180.0);
    EXPECT_DOUBLE_EQ(actPosX, 10.0);
    EXPECT_DOUBLE_EQ(actPosY, -2.5);
    EXPECT_DOUBLE_EQ(actRot, 180.0);

    sentSpiData = spi->getSendLine();
    ASSERT_EQ(sentSpiData.size(), 19);
    EXPECT_EQ(sentSpiData[0], 0xab);

    EXPECT_TRUE(spi->clearBuffor());
    EXPECT_TRUE(gpio->clearBuffor());
}

TEST(ManagerSPI, init){TEST_LOG_HEADER
    auto gpio = Drivers::createDriverGPIO({22}).at(22);
    auto spi = Drivers::createDriverSPI();
    gpio->setPinReading({
        false, true
    });
    spi->setReciveBuffor({
        {0x25},
    });
    {
        ManagerSPI manager("/dev/spidev0.0", 22, ManagerSPI::StrategySequence{
            std::shared_ptr<StrategySpi::StrategySpiIfc>(new StrategySpi::TransmissionKeyStrategy(0x25, 0xab))});
        manager.init();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    auto sentSpiData = spi->getSendLine();
    ASSERT_EQ(sentSpiData.size(), 1);
    EXPECT_EQ(sentSpiData[0], 0xab);
    EXPECT_TRUE(spi->clearBuffor());
    EXPECT_TRUE(gpio->clearBuffor());
}
