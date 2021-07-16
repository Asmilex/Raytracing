cmake_minimum_required(VERSION 3.7)
project(RT_in_one_weekend)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/src)

set( executableOutputPath ${PROJECT_SOURCE_DIR}/../bin)

add_executable(RT_in_one_weekend ${PROJECT_SOURCE_DIR}/../src/main.cpp)
