#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>

struct CpuStats {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    double usage_percent;
    std::chrono::steady_clock::time_point timestamp;
};

struct ProcessInfo {
    int pid;
    std::string name;
    char state;
    unsigned long long utime, stime;
    long long rss_kb;
    double cpu_percent;
    std::chrono::steady_clock::time_point timestamp;
};

struct MemInfo {
    unsigned long long mem_total_kb;
    unsigned long long mem_free_kb;
    unsigned long long mem_available_kb;
    unsigned long long buffers_kb;
    unsigned long long cached_kb;
    double usage_percent;
};

class ProcReader {
public:
    static CpuStats readCpuStats();
    static std::vector<ProcessInfo> readProcesses(int limit = -1);
    static MemInfo readMemInfo();
    static double calculateCpuUsage(const CpuStats &prev, const CpuStats &curr);
};
