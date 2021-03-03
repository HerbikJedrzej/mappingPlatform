#include <gtest/gtest.h>
#include <MemoryComparator.hh>
#include <I2C_driver_Mock.hh>
#include <GPIO_driver_Mock.hh>

namespace {

class TestHelper_MemoryComparator : public MemoryComparator{
public:
    TestHelper_MemoryComparator():
        MemoryComparator("./configs/stmPlatform_test.bin", "/dev/i2c-1", 0x50, 17, 4){}
    SpiInterfaceMode run() override{
        process        (0x0000, "EEprom_key", jsonProvider.eeprom_key());
        process        (0x000a, "Spi recived key", jsonProvider.spi_recived_key());
        process<double>(0x000b, "Stright movement P", jsonProvider.stright_movement__p());
        return jsonProvider.spiInterfaceMode();
    }
};

}

TEST(MemoryComparator, run){TEST_LOG_HEADER
    auto i2c = Drivers::createDriver();
    auto gpio = Drivers::createDriverGPIO({4, 17});
    i2c->setReadingBuffor({
        Drivers::I2Cdata{0x0000, {21, 38, 55, 72, 89, 106, 125, 140, 157, 174}},
        Drivers::I2Cdata{0x000a, {90}},
        Drivers::I2Cdata{0x000b, {154, 153, 153, 153, 153, 153, 184, 63}},
    });
    {
        TestHelper_MemoryComparator comparator;
        comparator.run();
    }
    auto data = i2c->getWritieLine();
    ASSERT_EQ(data.data.size(), 10);
    EXPECT_EQ(data.data[0], 0x15);
    EXPECT_EQ(data.data[1], 0x26);
    EXPECT_EQ(data.data[2], 0x37);
    EXPECT_EQ(data.data[3], 0x48);
    EXPECT_EQ(data.data[4], 0x59);
    EXPECT_EQ(data.data[5], 0x6a);
    EXPECT_EQ(data.data[6], 0x7b);
    EXPECT_EQ(data.data[7], 0x8c);
    EXPECT_EQ(data.data[8], 0x9d);
    EXPECT_EQ(data.data[9], 0xae);
    data = i2c->getWritieLine();
    ASSERT_EQ(data.data.size(), 8);
    EXPECT_EQ(data.data[0], 154);
    EXPECT_EQ(data.data[1], 153);
    EXPECT_EQ(data.data[2], 153);
    EXPECT_EQ(data.data[3], 153);
    EXPECT_EQ(data.data[4], 153);
    EXPECT_EQ(data.data[5], 153);
    EXPECT_EQ(data.data[6], 185);
    EXPECT_EQ(data.data[7], 63);
    EXPECT_TRUE(i2c->clearBuffor());
    auto gpioStates = gpio[4]->getPinSetting();
    ASSERT_EQ(gpioStates.size(), 2);
    EXPECT_TRUE(gpioStates[0]);
    EXPECT_FALSE(gpioStates[1]);
    EXPECT_TRUE(gpio[4]->clearBuffor());
    gpioStates = gpio[17]->getPinSetting();
    ASSERT_EQ(gpioStates.size(), 6);
    EXPECT_TRUE (gpioStates[0]);
    EXPECT_FALSE(gpioStates[1]);
    EXPECT_TRUE (gpioStates[2]);
    EXPECT_FALSE(gpioStates[3]);
    EXPECT_TRUE (gpioStates[4]);
    EXPECT_TRUE (gpioStates[5]);
    EXPECT_TRUE(gpio[17]->clearBuffor());
}
