set(LIB_NAME BME280_driver)

aux_source_directory(. SRC_FILES)

add_library(${LIB_NAME} INTERFACE)
target_sources(${LIB_NAME} INTERFACE
                ${SRC_FILES})

target_include_directories(${LIB_NAME} INTERFACE .)