cmake_minimum_required(VERSION 3.10.0)

set(name robotLib)

# Add local sources

include(src.cmake)

# define target

add_library(${name} STATIC ${SOURCE_PATHS})
target_link_libraries(${name} LINK_PUBLIC pthread)
set_property(TARGET ${name} PROPERTY CXX_STANDARD 17)

target_compile_options(${name} PRIVATE ${COMMON_FLAGS})
target_compile_options(${name} INTERFACE ${COMMON_FLAGS})

separate_arguments(ADDITIONAL_FLAGS)
target_compile_options(${name} PRIVATE ${ADDITIONAL_FLAGS})
target_compile_options(${name} INTERFACE ${ADDITIONAL_FLAGS})

# Addlocal headers

target_include_directories(${name} PRIVATE ${INCLUDE_PATHS})
target_include_directories(${name} INTERFACE ${INCLUDE_PATHS})

target_include_directories(${name} PRIVATE   ${CMAKE_CURRENT_BINARY_DIR}/../inc_generated)
target_include_directories(${name} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/../inc_generated)
target_include_directories(${name} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/../inc_generated/flatbuffers)
target_include_directories(${name} INTERFACE ${FLATBUFFERS_INCLUDE_DIRS})
