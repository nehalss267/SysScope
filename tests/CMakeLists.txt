find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(syscope_tests ProcReaderTest.cpp)
target_link_libraries(syscope_tests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} syscope pthread)
add_test(NAME SysScopeTests COMMAND syscope_tests)
