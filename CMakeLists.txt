cmake_minimum_required(VERSION 3.14)
project(HashBench)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(yaml-cpp REQUIRED)

add_library(framework_lib STATIC
    src/core/ConfigLoader.cpp
    src/core/Registry.cpp
    src/core/ScoreFunction.cpp
)

target_include_directories(framework_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(hashbench
    cli/main.cpp

    cli/dataset/CSVDataset.cpp
    
    cli/metric/CollisionRate.cpp
    cli/metric/Speed.cpp
    cli/metric/Entropy.cpp
    cli/metric/PearsonChi2.cpp
)

target_include_directories(hashbench PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/cli
)

target_link_libraries(hashbench PRIVATE framework_lib yaml-cpp)