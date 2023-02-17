set(LIB_NAME board_dep)
aux_source_directory(. SRC_FILES)

add_library(${LIB_NAME} INTERFACE)

target_sources(${LIB_NAME} INTERFACE
                ${SRC_FILES})

target_link_libraries(${LIB_NAME} INTERFACE
                        BME280_driver
                        hardware_interp
                        hardware_i2c)

target_include_directories(${LIB_NAME} INTERFACE .)