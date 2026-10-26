add_executable(bench_syscope bench_main.cpp)
target_link_libraries(bench_syscope PRIVATE syscope pthread)
