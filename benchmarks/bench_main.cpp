#include "ProcReader.h"
#include "ThreadPool.h"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "Running benchmarks..." << std::endl;
    
    // Single-threaded benchmark
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        ProcReader::readCpuStats();
        ProcReader::readMemInfo();
        ProcReader::readProcesses(50);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto st_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Single-threaded (100 iterations): " << st_duration.count() << "ms" << std::endl;
    
    // Multi-threaded benchmark
    start = std::chrono::high_resolution_clock::now();
    ThreadPool pool(4);
    for (int i = 0; i < 100; ++i) {
        pool.enqueue([]() {
            ProcReader::readCpuStats();
            ProcReader::readMemInfo();
            ProcReader::readProcesses(50);
        });
    }
    pool.shutdown();
    end = std::chrono::high_resolution_clock::now();
    auto mt_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Multi-threaded with ThreadPool (100 tasks): " << mt_duration.count() << "ms" << std::endl;
    
    return 0;
}
