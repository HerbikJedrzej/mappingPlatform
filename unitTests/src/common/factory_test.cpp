#include <gtest/gtest.h>
#include <SPI_driver_Mock.hh>
#include <I2C_driver_Mock.hh>
#include <GPIO_driver_Mock.hh>
#include <factory.hh>

TEST(factory, cycle){TEST_LOG_HEADER
    auto gpio = Drivers::createDriverGPIO({0, 1, 4, 7, 12, 17, 18, 22});
    auto i2c = Drivers::createDriver();
    auto spi = Drivers::createDriverSPI();
    gpio.at(22)->setPinReading({true, true});
    spi->setReciveBuffor({{0x5a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    i2c->setReadingBuffor({
        Drivers::I2Cdata{0x0000, {21, 38, 55, 72, 89, 106, 123, 140, 157, 174}},
        Drivers::I2Cdata{0x000a, {150}},
        Drivers::I2Cdata{0x000b, {90}},
        Drivers::I2Cdata{0x000c, {0}},
        Drivers::I2Cdata{0x0100, {154, 153, 153, 153, 153, 153, 241, 63}},
        Drivers::I2Cdata{0x0108, {205, 204, 204, 204, 204, 204,   0, 64}},
        Drivers::I2Cdata{0x0110, {205, 204, 204, 204, 204, 204,   8, 64}},
        Drivers::I2Cdata{0x0118, {102, 102, 102, 102, 102, 102,  16, 64}},
        Drivers::I2Cdata{0x0120, { 51,  51,  51,  51,  51,  51, 243, 63}},
        Drivers::I2Cdata{0x0128, {154, 153, 153, 153, 153, 153,   1, 64}},
        Drivers::I2Cdata{0x0130, {154, 153, 153, 153, 153, 153,   9, 64}},
        Drivers::I2Cdata{0x0138, {205, 204, 204, 204, 204, 204,  16, 64}},
        Drivers::I2Cdata{0x0203, {2}},
        Drivers::I2Cdata{0x0204, {0, 0, 0, 0, 0, 0, 22, 64}},
        Drivers::I2Cdata{0x020c, {0xff, 0xf, 0, 0}},
        Drivers::I2Cdata{0x0210, {103,  43, 0, 0}},
        Drivers::I2Cdata{0x0214, {206,  86, 0, 0}},
        Drivers::I2Cdata{0x0218, { 53, 130, 0, 0}},
        Drivers::I2Cdata{0x021c, {156, 173, 0, 0}},
        // Drivers::I2Cdata{0x0220, {0, 0, 0, 0}},
        // Drivers::I2Cdata{0x0224, {0, 0, 0, 0}},

    });
    Factory factory("./configs/raspberryPlatform_factory_test.bin", "./configs/stmPlatform_factory_test.bin");
    EXPECT_NO_THROW(
        auto platform = factory.clearAndCreate();
        EXPECT_DOUBLE_EQ(platform->currentPosition.x,     platform->expectedPosition.x);
        EXPECT_DOUBLE_EQ(platform->currentPosition.y,     platform->expectedPosition.y);
        EXPECT_DOUBLE_EQ(platform->currentPosition.angle, platform->expectedPosition.angle);
        platform->expectedPosition.x = 25.63;
        platform->expectedPosition.y = -2125.63;
        platform->expectedPosition.angle = 95.4931640625;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        EXPECT_NE(platform->currentPosition.x,     platform->expectedPosition.x);
        EXPECT_NE(platform->currentPosition.y,     platform->expectedPosition.y);
        EXPECT_NE(platform->currentPosition.angle, platform->expectedPosition.angle);
        auto sentSpiData = spi->getSendLine();
        sentSpiData[0] = 0x5a;
        ASSERT_EQ(sentSpiData.size(), 19);
        spi->setReciveBuffor({sentSpiData});
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        EXPECT_DOUBLE_EQ(platform->currentPosition.x,     platform->expectedPosition.x);
        EXPECT_DOUBLE_EQ(platform->currentPosition.y,     platform->expectedPosition.y);
        EXPECT_DOUBLE_EQ(platform->currentPosition.angle, platform->expectedPosition.angle);
        EXPECT_DOUBLE_EQ(platform->currentPosition.x,  25.63);
        EXPECT_DOUBLE_EQ(platform->currentPosition.y, -2125.63);
        EXPECT_DOUBLE_EQ(platform->currentPosition.angle,  95.4931640625);
    );

    auto sentSpiData = spi->getSendLine();
    ASSERT_EQ(sentSpiData.size(), 19);
    EXPECT_TRUE(i2c->clearBuffor());
    EXPECT_TRUE(spi->clearBuffor());
    auto states = gpio.at(17)->getPinSetting();
    EXPECT_EQ(states.size(), 2);
    states = gpio.at(0)->getPinSetting();
    EXPECT_EQ(states.size(), 2);
    states = gpio.at(4)->getPinSetting();
    EXPECT_EQ(states.size(), 2);
    EXPECT_TRUE(gpio.at(0)->clearBuffor());
    EXPECT_TRUE(gpio.at(1)->clearBuffor());
    EXPECT_TRUE(gpio.at(4)->clearBuffor());
    EXPECT_TRUE(gpio.at(7)->clearBuffor());
    EXPECT_TRUE(gpio.at(12)->clearBuffor());
    EXPECT_TRUE(gpio.at(17)->clearBuffor());
    EXPECT_TRUE(gpio.at(18)->clearBuffor());
    EXPECT_TRUE(gpio.at(22)->clearBuffor());
}
