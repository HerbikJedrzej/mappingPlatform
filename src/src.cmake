macro(addFlag flag)
  if(NOT ADDITIONAL_FLAGS MATCHES ${flag})
    set(ADDITIONAL_FLAGS "${ADDITIONAL_FLAGS} ${flag}")
  endif()
endmacro()

addFlag("-lm")
addFlag("-pthread")

list(APPEND INCLUDE_PATHS
  EEprom
  STM_interface
  STM_interface/strategies
  common
  HwDrivers
)

list(APPEND SOURCE_PATHS
  common/factory.cpp
  common/logSystem.cpp
  EEprom/JsonConfigProvider.cpp
  EEprom/MemoryComparator.cpp
  STM_interface/BusManager.cpp
  STM_interface/strategies/AllDataUsed_Strategy.cpp
  STM_interface/strategies/Battery_Strategy.cpp
  STM_interface/strategies/DistanceMeasure_Strategy.cpp
  STM_interface/strategies/Double_Strategy.cpp
  STM_interface/strategies/Position_Strategy.cpp
  STM_interface/strategies/TransmissionKey_Strategy.cpp
)
