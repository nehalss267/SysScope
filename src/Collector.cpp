#include "Collector.h"
#include <iostream>

Collector::Collector(std::chrono::milliseconds interval)
    : interval_(interval), running_(false), stop_requested_(false) {
}

Collector::~Collector() {
    stop();
}

void Collector::start() {
    if (running_) return;
    stop_requested_ = false;
    running_ = true;
    worker_ = std::thread(&Collector::collectLoop, this);
}

void Collector::stop() {
    if (!running_) return;
    stop_requested_ = true;
    if (worker_.joinable()) {
        worker_.join();
    }
    running_ = false;
}

void Collector::setCallback(Callback cb) {
    std::lock_guard<std::mutex> lock(mutex_);
    callback_ = cb;
}

bool Collector::isRunning() const {
    return running_;
}

void Collector::collectLoop() {
    CpuStats prev_cpu = ProcReader::readCpuStats();
    std::this_thread::sleep_for(interval_);
    
    while (!stop_requested_) {
        CpuStats curr_cpu = ProcReader::readCpuStats();
        curr_cpu.usage_percent = ProcReader::calculateCpuUsage(prev_cpu, curr_cpu);
        prev_cpu = curr_cpu;
        
        auto processes = ProcReader::readProcesses(50);
        auto mem = ProcReader::readMemInfo();
        
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (callback_) {
                callback_(curr_cpu, processes, mem);
            }
        }
        
        std::this_thread::sleep_for(interval_);
    }
}
