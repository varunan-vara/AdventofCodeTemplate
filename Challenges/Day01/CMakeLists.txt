# cmake_minimum_required(VERSION 3.10)
# project(AdevntOfCode VERSION 2025.0 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(SOURCE_FILE ${TARGET_NAME}.cpp)

add_executable(${TARGET_NAME} ${SOURCE_FILE})


target_include_directories(${TARGET_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/Utils
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)