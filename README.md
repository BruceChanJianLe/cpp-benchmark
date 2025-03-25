## Introduction

This repository is a template or a starting point that is:
- A CMake Project
- Auto clone googletest & googlebenchmark
- Populated with examples

Need not worry about setting everything from scratch.

## Usage

1. Clone repo shallowly.
```bash
git clone --depth 1 https://github.com/BruceChanJianLe/cpp-benchmark.git
```
1. Populate `src` and `include` directory with your testing code.
1. Add your benchmark cases in the `benchmark` directory.

## Building

Build using the `cmake` command.  
```bash
cmake -S . -B build
cmake --build build
```

## Running

Run the respective executable in the `build` directory.  
```bash
./build/benchmark/vector_benchmark
```

## Extras

To disable cpu scaling for benchmark measurements, run the following:  
```bash
# disable cpu scaling
sudo cpupower frequency-set --governor performance 
```

To enable it after testing, run the following:  
```bash
# enable cpu scaling
sudo cpupower frequency-set --governor powersave
```

Beyond bendmarking, you can also check the cache locality with `perf`.  
```bash
perf stat -d -d -d ./build/benchmark/game_entity_benchmark --benchmark_filter=OOPGameEntityEnableRender/65536 --benchmark_min_time=3
```

Oops! You may encounter error regarding `perf_event_paranoid` setting is 4...
The `perf_event_paranoid` determines the level of access of performance
monitoring events. 

```bash
# View current perf_event_paranoid setting
cat /proc/sys/kernel/perf_event_paranoid
```

To temporarily change it, you can run:  
```bash
sudo sysctl -w kernel/perf_event_paranoid=-1
# 1: Allow use of (almost) all events by all users
#    Ignore mlock limit after perf_event_mlock_kb without CAP_IPC_LOCK
# 0: Disallow raw and ftrace function tracepoint access
# 1: Disallow CPU event access
# 2: Disallow kernel profiling
```

For **permanent** changes, edit this file:  
```bash
sudoedit /etc/sysctl.conf
# kernel.perf_event_paranoid=-1
sudo sysctl -p
# Apply the changes
```

Be mindful that lowering the perf_event_paranoid setting increases the access
to performance events and profiling capabilities, which can be a potential
security risk, especially on multi-user systems. Adjust the setting according
to your use case and security requirements.  

Now, finally you can view the different between the two!
```bash
perf stat -d -d -d ./build/benchmark/game_entity_benchmark --benchmark_filter=OOPGameEntityEnableRender/65536 --benchmark_min_time=3
perf stat -d -d -d ./build/benchmark/game_entity_benchmark --benchmark_filter=DODGameEntityEnableRender/65536 --benchmark_min_time=3
```

## Good Reads

[1](https://bencher.dev/learn/benchmarking/cpp/google-benchmark/)
[2](https://jaredmil.medium.com/micro-benchmarking-c-with-quick-bench-8788b1edcf79)
[3](https://jaredmil.medium.com/c-beyond-the-syllabus-2-intro-to-benchmarking-and-macro-benchmarking-deep-dive-a465fc73a604)
