# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/nte/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(bme_280_lib C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(bme_280_lib bme_280_lib.c)

pico_set_program_name(bme_280_lib "bme_280_lib")
pico_set_program_version(bme_280_lib "0.1")

pico_enable_stdio_uart(bme_280_lib 1)
pico_enable_stdio_usb(bme_280_lib 1)

add_subdirectory(BME280_driver)
add_subdirectory(pico_driver)

# Add the standard library to the build
target_link_libraries(bme_280_lib
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(bme_280_lib PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(bme_280_lib
        hardware_spi
        hardware_i2c
        hardware_interp
        hardware_timer
        hardware_clocks
        board_dep
        )

pico_add_extra_outputs(bme_280_lib)

