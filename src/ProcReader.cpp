#include "ProcReader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>

CpuStats ProcReader::readCpuStats() {
    CpuStats stats = {};
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    std::string cpu;
    ss >> cpu >> stats.user >> stats.nice >> stats.system >> stats.idle 
       >> stats.iowait >> stats.irq >> stats.softirq >> stats.steal;
    stats.usage_percent = 0.0;
    stats.timestamp = std::chrono::steady_clock::now();
    return stats;
}

double ProcReader::calculateCpuUsage(const CpuStats &prev, const CpuStats &curr) {
    unsigned long long prev_idle = prev.idle + prev.iowait;
    unsigned long long curr_idle = curr.idle + curr.iowait;
    
    unsigned long long prev_nonidle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    unsigned long long curr_nonidle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;
    
    unsigned long long prev_total = prev_idle + prev_nonidle;
    unsigned long long curr_total = curr_idle + curr_nonidle;
    
    unsigned long long total_diff = curr_total - prev_total;
    unsigned long long idle_diff = curr_idle - prev_idle;
    
    if (total_diff == 0) return 0.0;
    return 100.0 * (total_diff - idle_diff) / total_diff;
}

std::vector<ProcessInfo> ProcReader::readProcesses(int limit) {
    std::vector<ProcessInfo> processes;
    DIR* dir = opendir("/proc");
    if (!dir) return processes;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type != DT_DIR) continue;
        std::string name = entry->d_name;
        if (name.find_first_not_of("0123456789") != std::string::npos) continue;
        
        int pid = std::stoi(name);
        std::string stat_path = "/proc/" + name + "/stat";
        std::ifstream stat_file(stat_path);
        if (!stat_file.is_open()) continue;
        
        std::string stat_line;
        std::getline(stat_file, stat_line);
        
        size_t start = stat_line.find('(');
        size_t end = stat_line.rfind(')');
        if (start == std::string::npos || end == std::string::npos) continue;
        
        ProcessInfo info;
        info.pid = pid;
        info.name = stat_line.substr(start + 1, end - start - 1);
        std::string rest = stat_line.substr(end + 2);
        std::stringstream ss(rest);
        ss >> info.state >> info.utime >> info.stime;
        info.rss_kb = 0;
        info.cpu_percent = 0.0;
        info.timestamp = std::chrono::steady_clock::now();
        processes.push_back(info);
        
        if (limit > 0 && (int)processes.size() >= limit) break;
    }
    closedir(dir);
    return processes;
}

MemInfo ProcReader::readMemInfo() {
    MemInfo mem = {};
    std::ifstream file("/proc/meminfo");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        unsigned long long val;
        std::string unit;
        ss >> key >> val >> unit;
        if (key == "MemTotal:") mem.mem_total_kb = val;
        else if (key == "MemFree:") mem.mem_free_kb = val;
        else if (key == "MemAvailable:") mem.mem_available_kb = val;
        else if (key == "Buffers:") mem.buffers_kb = val;
        else if (key == "Cached:") mem.cached_kb = val;
    }
    if (mem.mem_total_kb > 0) {
        mem.usage_percent = 100.0 * (mem.mem_total_kb - mem.mem_available_kb) / mem.mem_total_kb;
    } else {
        mem.usage_percent = 0.0;
    }
    return mem;
}
