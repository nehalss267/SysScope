# SysScope

Multithreaded Linux Resource Profiler

## Features
- CPU utilization monitoring via /proc/stat
- Process state and info tracking
- Memory consumption monitoring via /proc/meminfo
- Concurrent metric collectors using POSIX threads
- Thread synchronization with mutexes
- ThreadPool for parallel collection
- Safe shutdown mechanisms

## Build
```bash
mkdir build && cd build
cmake ..
make
```

## Run
```bash
./syscope_app
```

## Tests
```bash
ctest --output-on-failure
```

## Benchmarks
```bash
./benchmarks/bench_syscope
```

## Requirements
- Linux
- C++17
- CMake 3.14+
- pthread
- GoogleTest (for tests)
