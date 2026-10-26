#include "Collector.h"
#include <iostream>
#include <iomanip>

int main(int argc, char** argv) {
    std::cout << "SysScope - Linux Resource Profiler" << std::endl;
    
    Collector collector(std::chrono::milliseconds(1000));
    
    collector.setCallback([](const CpuStats& cpu, const std::vector<ProcessInfo>& procs, const MemInfo& mem) {
        std::cout << "\n=== System Resources ===" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "CPU Usage: " << cpu.usage_percent << "%" << std::endl;
        std::cout << "Memory Usage: " << mem.usage_percent << "% (" 
                  << (mem.mem_total_kb - mem.mem_available_kb) / 1024 << "MB / "
                  << mem.mem_total_kb / 1024 << "MB)" << std::endl;
        std::cout << "\nTop Processes:" << std::endl;
        for (size_t i = 0; i < std::min(procs.size(), size_t(10)); ++i) {
            std::cout << "  [" << procs[i].pid << "] " << procs[i].name 
                      << " (" << procs[i].state << ")" << std::endl;
        }
    });
    
    collector.start();
    
    std::cout << "Press Enter to stop..." << std::endl;
    std::cin.get();
    
    collector.stop();
    std::cout << "Stopped." << std::endl;
    
    return 0;
}
