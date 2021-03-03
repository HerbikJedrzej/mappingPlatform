cmake_minimum_required(VERSION 3.10.0)

set(name robotApp)

# Add local sources

list(APPEND SOURCES
  main.cpp
  GPIO_driver.cpp
  I2C_driver.cpp
  SPI_driver.cpp
)

# define target

add_executable(${name} ${SOURCES})
target_link_libraries(${name} LINK_PRIVATE robotLib)
set_property(TARGET ${name} PROPERTY CXX_STANDARD 17)
