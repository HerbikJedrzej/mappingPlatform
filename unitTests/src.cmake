list(APPEND TESTS_SRC
    main_test.cpp
    mocks/GPIO_driver.cpp
    mocks/I2C_driver.cpp
    mocks/SPI_driver.cpp
    mocks/TestDelay.cpp
    src/common/factory_test.cpp
    src/EEprom/JsonConfigProvider_test.cpp
    src/EEprom/MemoryComparator_test.cpp
    src/Matrix_test.cpp
    src/STM_interface/BusManager_test.cpp
    src/STM_interface/strategies/AllDataUsed_Strategy_test.cpp
    src/STM_interface/strategies/Battery_Strategy_test.cpp
    src/STM_interface/strategies/DistanceMeasure_Strategy_test.cpp
    src/STM_interface/strategies/Double_Strategy_test.cpp
    src/STM_interface/strategies/Position_Strategy_test.cpp
    src/STM_interface/strategies/TransmissionKey_Strategy_test.cpp
)