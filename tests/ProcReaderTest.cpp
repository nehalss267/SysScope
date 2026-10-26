#include <gtest/gtest.h>
#include "ProcReader.h"

TEST(ProcReaderTest, ReadCpuStats) {
    CpuStats stats = ProcReader::readCpuStats();
    // Basic sanity checks
    EXPECT_GE(stats.user + stats.nice + stats.system, 0ULL);
    EXPECT_GE(stats.idle, 0ULL);
}

TEST(ProcReaderTest, ReadMemInfo) {
    MemInfo mem = ProcReader::readMemInfo();
    EXPECT_GT(mem.mem_total_kb, 0ULL);
    EXPECT_LE(mem.mem_free_kb, mem.mem_total_kb);
    EXPECT_GE(mem.usage_percent, 0.0);
    EXPECT_LE(mem.usage_percent, 100.0);
}

TEST(ProcReaderTest, ReadProcesses) {
    auto processes = ProcReader::readProcesses(10);
    EXPECT_GE(processes.size(), 0);
    for (const auto &p : processes) {
        EXPECT_GT(p.pid, 0);
        EXPECT_FALSE(p.name.empty());
    }
}

TEST(ProcReaderTest, CalculateCpuUsage) {
    CpuStats prev = {100, 0, 50, 850, 0, 0, 0, 0, 0.0, std::chrono::steady_clock::now()};
    CpuStats curr = {150, 0, 75, 875, 0, 0, 0, 0, 0.0, std::chrono::steady_clock::now()};
    double usage = ProcReader::calculateCpuUsage(prev, curr);
    EXPECT_GE(usage, 0.0);
    EXPECT_LE(usage, 100.0);
}
