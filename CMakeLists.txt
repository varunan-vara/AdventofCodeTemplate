cmake_minimum_required(VERSION 3.10)
project(AdevntOfCode VERSION 2025.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# The following statement will compile all folders with another 
# CMakeLists.txt file, not just 'DayX' ones
file(GLOB DAYS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/Challenges
    Challenges/*
)

foreach (DIR ${DAYS})
    # Check if the directory has a CMakeLists.txt
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Challenges/${DIR}/CMakeLists.txt)
        message (STATUS "Compiling folder: ${DIR}")
        add_subdirectory(Challenges/${DIR} build/${DIR})
    else()
        message (WARNING "Skipping folder: ${DIR}")
    endif()
endforeach()