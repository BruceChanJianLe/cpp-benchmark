## Introduction

This repository is a template or a starting point that is:
- A CMake Project
- Auto clone googletest & googlebenchmark
- Populated with examples

Need not worry about setting everything from scratch.

## Usage

1. Populate `src` and `include` directory with your testing code.
1. Add your benchmark cases in the `benchmark` directory.

## Building

Build using the `cmake` command.  
```bash
cmake -S . -B build
cmake --build build
```

## Running

I use ctest as a helper to run the unit test.
More about it [here](https://github.com/BruceChanJianLe/ctest).
the ctest command.

```bash
ctest --test-dir build
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

