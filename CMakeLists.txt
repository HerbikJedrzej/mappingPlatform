cmake_minimum_required(VERSION 3.0.2)
project(mapingRobot CXX)

macro(addOptionFlag flag ONorOFF)
  option(Flag_${flag} "Flag ${Flag_${flag}}:" ${ONorOFF})
  message(STATUS "Flag_${flag}: ${Flag_${flag}}")
  if(Flag_${flag})
    list(APPEND COMMON_FLAGS -${flag})
  endif()
endmacro()

addOptionFlag(Wall ON)
addOptionFlag(pedantic ON)
addOptionFlag(Waddress ON)
addOptionFlag(zdefs ON)
addOptionFlag(Wno-psabi ON)
addOptionFlag(Wunused-parameter ON)
addOptionFlag(Wextra ON)
addOptionFlag(Werror OFF)

add_subdirectory(src)
add_subdirectory(robotApp)
add_subdirectory(unitTests)

# create list of configs

include(configs.cmake)
foreach(FILE ${CONFIGS_FILES})
set(CONFIGS_LIST "${CONFIGS_LIST} ${FILE}")
endforeach()
message(INFO ": Use configure files:\n${CONFIGS_LIST}")

# create target for generation bin files

add_custom_target(configs
    COMMAND 
      echo ${CONFIGS_LIST} | xargs -n1 | xargs -i sh -c 'flatc --binary -o ${CMAKE_CURRENT_BINARY_DIR}/configs  ${CMAKE_CURRENT_SOURCE_DIR}/schemas/{}* ${CMAKE_CURRENT_SOURCE_DIR}/configs/{}*'
      # echo ${CONFIGS_LIST} | xargs -n1    -----   print words, every in new line
      # xargs -i  sh -c 'command {}'        -----   execute command where {} is replaced by argument
    COMMENT "Generate configs bin."
  )
add_dependencies(robotApp configs)
add_dependencies(unitTests configs)

# create target for generation headers
add_custom_target(flatbuffers
  COMMAND
    flatc --cpp -o ${CMAKE_CURRENT_BINARY_DIR}/inc_generated ${CMAKE_CURRENT_SOURCE_DIR}/schemas/*
  COMMENT "Generate flatbuffers."
)

# copy flatbuffers headers

execute_process(
  COMMAND 
    mkdir inc_generated configs
  COMMAND
    ln -s ${CMAKE_CURRENT_BINARY_DIR}/configs unitTests/configs
  COMMAND
    cp -r ${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers ./inc_generated/.
)

# create init config headers. Could be recreate by target 'flatbuffers'

message(INFO " Generate flatbuffers.")
foreach(FILE ${CONFIGS_FILES})
execute_process(
  COMMAND
    flatc --cpp -o ${CMAKE_CURRENT_BINARY_DIR}/inc_generated ${CMAKE_CURRENT_SOURCE_DIR}/schemas/${FILE}.fbs
  )
endforeach()
