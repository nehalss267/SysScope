cmake_minimum_required(VERSION 3.14)
project(SysScope LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(syscope
    src/ProcReader.cpp
    src/Collector.cpp
    src/ThreadPool.cpp
)

target_include_directories(syscope PUBLIC include)

add_executable(syscope_app src/main.cpp)
target_link_libraries(syscope_app PRIVATE syscope pthread)

enable_testing()
add_subdirectory(tests)

add_subdirectory(benchmarks)
