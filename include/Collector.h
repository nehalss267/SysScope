#pragma once

#include "ProcReader.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <functional>

class Collector {
public:
    using Callback = std::function<void(const CpuStats&, const std::vector<ProcessInfo>&, const MemInfo&)>;

    Collector(std::chrono::milliseconds interval = std::chrono::milliseconds(1000));
    ~Collector();

    void start();
    void stop();
    void setCallback(Callback cb);
    bool isRunning() const;

private:
    void collectLoop();

    std::chrono::milliseconds interval_;
    Callback callback_;
    std::thread worker_;
    std::mutex mutex_;
    std::atomic<bool> running_;
    std::atomic<bool> stop_requested_;
};
