cmake_minimum_required(VERSION 3.15)
project(HashBench)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE CORE_SRC src/core/*.cpp)
file(GLOB_RECURSE HASH_SRC src/hash/*.cpp)
file(GLOB_RECURSE METRIC_SRC src/metric/*.cpp)
file(GLOB_RECURSE DATASET_SRC src/dataset/*.cpp)
file(GLOB_RECURSE CLI_SRC cli/*.cpp)

add_executable(hashbench
    ${CORE_SRC}
    ${HASH_SRC}
    ${METRIC_SRC}
    ${DATASET_SRC}
    ${CLI_SRC}
)

target_compile_definitions(hashbench PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)