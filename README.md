# Cache Simulator

## Overview

This program simulates two types of cache architectures:
1. **Direct-Mapped Cache**
2. **Fully Associative Cache**

The simulation includes a variety of memory access patterns to evaluate cache performance based on hit rates.

## Files

- `cache.cpp`: Contains the implementation of the cache simulation, including:
  - Cache configurations and settings.
  - Memory address generators for testing.
  - Cache simulation functions for both direct-mapped and fully associative caches.
  - Main function to drive the simulation and print results.
- `P2-Report.pdf`: Provides a detailed explanation of the project, including:
  - Breakdown of our test cases used in the cache simulations.
  - Results and analyses of the experiments.
  - Conclusions drawn from the simulation data.
## Building

To build the program, use a C++ compiler. For example, using `g++`:

```bash
g++ -o cache_simulator cache_simulator.cpp
```
```bash
./cache_simulator
```
### Instructions

1. **Choose Cache Type:**
   - Enter `1` for Fully Associative Cache.
   - Enter `2` for Direct-Mapped Cache.

2. **Choose Line Size:**
   - Enter one of the following values: `16`, `32`, `64`, `128`.

The program will simulate cache accesses based on your selections and display hit/miss results.
### Example Output
```bash
Choose either FA(1) or DM(2): 1
Choose the line size of the cache (16,32,64,128): 64
Fully Associative Cache Simulator
0x00000000 (Miss)  1
0x00000020 (Miss)  2
0x00000040 (Hit)   3
...
Hit ratio = 12.345678
```


