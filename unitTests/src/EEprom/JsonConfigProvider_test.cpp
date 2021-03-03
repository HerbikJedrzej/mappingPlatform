#include <gtest/gtest.h>
#include <JsonConfigProvider.hh>

namespace JsonConfigProviderTesthelper{
    union ConverseDouble{
        double val;
        uint8_t array[8];
    };
    
}

TEST(JsonConfigProvider, stright_movement__p){TEST_LOG_HEADER
	JsonConfigProvider config("./configs/stmPlatform_test.bin");
    JsonConfigProviderTesthelper::ConverseDouble converter;
    converter.val = 0.1;
    auto data = config.stright_movement__p();
    ASSERT_EQ(8, data.size());
    EXPECT_EQ(converter.array[0], data[0]);
    EXPECT_EQ(converter.array[1], data[1]);
    EXPECT_EQ(converter.array[2], data[2]);
    EXPECT_EQ(converter.array[3], data[3]);
    EXPECT_EQ(converter.array[4], data[4]);
    EXPECT_EQ(converter.array[5], data[5]);
    EXPECT_EQ(converter.array[6], data[6]);
    EXPECT_EQ(converter.array[7], data[7]);
}

TEST(JsonConfigProvider, spi_interface_mode){TEST_LOG_HEADER
	JsonConfigProvider config("./configs/stmPlatform_test.bin");
    auto data = config.spi_interface_mode();
    ASSERT_EQ(1, data.size());
    EXPECT_DOUBLE_EQ(data[0], 1);
}

TEST(JsonConfigProvider, stright_movement__d){TEST_LOG_HEADER
	JsonConfigProvider config("./configs/stmPlatform_test.bin");
    JsonConfigProviderTesthelper::ConverseDouble converter;
    auto data = config.stright_movement__d();
    ASSERT_EQ(8, data.size());
    for(unsigned int i = 0; i < data.size(); i++)
        converter.array[i] = data[i];
    EXPECT_DOUBLE_EQ(converter.val, 124.1);
}

TEST(JsonConfigProvider, rotate_movement__anti_windup){TEST_LOG_HEADER
	JsonConfigProvider config("./configs/stmPlatform_test.bin");
    JsonConfigProviderTesthelper::ConverseDouble converter;
    auto data = config.rotate_movement__anti_windup();
    ASSERT_EQ(8, data.size());
    for(unsigned int i = 0; i < data.size(); i++)
        converter.array[i] = data[i];
    EXPECT_DOUBLE_EQ(converter.val, -4124.0);
}

TEST(JsonConfigProvider, spi_transmit_key){TEST_LOG_HEADER
	JsonConfigProvider config("./configs/stmPlatform_test.bin");
    auto data = config.spi_transmit_key();
    ASSERT_EQ(1, data.size());
    EXPECT_EQ(data[0], 0x96);
}

TEST(JsonConfigProvider, spi_recived_key){TEST_LOG_HEADER
	JsonConfigProvider config("./configs/stmPlatform_test.bin");
    auto data = config.spi_recived_key();
    ASSERT_EQ(1, data.size());
    EXPECT_EQ(data[0], 0x5a);
}
