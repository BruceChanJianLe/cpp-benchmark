## Introduction

A simple benchmark for using the standard parallel libary.

GNU: There is significant benefit from using std::execution::par_unseq.
![Image](https://github.com/user-attachments/assets/8d3f9f94-563b-4f86-ac2d-f41bfdc8a0bc)

Clang: No significant benefit, I believe it is still under development.
![Image](https://github.com/user-attachments/assets/02c9b7d0-a652-406f-b870-6f3323128464)

## Building

Building with GNU:
```bash
cmake -S . -B build
cmake --build build
```

Building with Clang:
```bash
export CC=clang
export CXX=clang++
cmake -S . -B build
cmake --build build
```

## Running

Run the respective executable in the `build` directory.  
```bash
./build/benchmark/std_par_benchmark
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
