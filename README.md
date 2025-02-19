# Cache Simulator Project

## Overview
This project involves the development of a basic cache simulator using C++. The simulator models cache behavior based on configurable parameters such as cache size, block size, associativity, replacement policy, and write policy. It processes memory access patterns provided in trace files to compute key performance metrics, including hit rates and memory access latencies. The simulator was used to evaluate the performance of various cache configurations across multiple workloads, generating insights into cache mechanisms and their impact on system performance.

## Key Features
- **Configurable Cache Parameters**: The simulator allows users to define cache configurations, including:
  - **Block Size**: Size of each cache block in bytes.
  - **Associativity**: Specifies the associativity of the cache (e.g., direct-mapped, 2-way, 4-way).
  - **Data Size**: Total size of the data in the cache.
  - **Replacement Policy**: Specifies the replacement policy (e.g., LRU, random).
  - **Miss Penalty**: Number of cycles penalized on a cache miss.
  - **Write Allocation**: Specifies the cache policy on store misses (Write-Allocate or No Write-Allocate).

- **Trace File Processing**: The simulator processes memory traces provided in trace files, where each line represents a memory reference with the following fields:
  - **Access Type**: Indicates whether the access is a load ('l') or a store ('s').
  - **Address**: A 32-bit integer specifying the memory address being accessed.
  - **Instructions since last memory access**: Number of instructions executed since the previous memory access.

- **Performance Metrics**: The simulator records key performance metrics for each configuration and workload, including:
  - **Total Hit Rate**: Percentage of memory accesses found in the cache.
  - **Load Hit Rate**: Hit rate specifically for load instructions.
  - **Store Hit Rate**: Hit rate specifically for store instructions.
  - **Total Run Time**: Time taken to execute the workload in CPU cycles.
  - **Average Memory Access Latency**: Average time required to access memory in CPU cycles.

## Workloads
The simulator was tested using standard workloads from common benchmarks:
- **gcc**: A compiler workload.
- **gzip**: A compression workload.
- **mcf**: A memory-intensive workload.
- **swim**: A floating-point computational workload.
- **twolf**: A CAD application workload.

## Results
The project evaluated various cache configurations, including direct-mapped caches (small, medium, large, mega) and set-associative caches (2-way and 4-way with LRU and random replacement policies). The results were analyzed to understand the impact of different cache designs on performance metrics. Key findings include:
- **High Hit Rates**: Workloads like gcc, swim, and twolf exhibited high hit rates across most configurations, often exceeding 90%.
- **Low Hit Rates**: The mcf workload showed very low hit rates in direct-mapped caches due to significant conflict misses.
- **Impact of Cache Size and Associativity**: Larger caches and higher associativity generally improved hit rates and reduced latency, especially for workloads with irregular access patterns like mcf.
- **Minimal Impact of Replacement and Write Policies**: Replacement and write allocation policies had limited impact on total hit rates and latency for most workloads.

## Visualizations
The results were visualized using bar charts to compare:
- **Total Hit Rates by Configuration and Trace**: Showed how hit rates varied across workloads and configurations.
- **Average Memory Access Latency by Configuration and Trace**: Illustrated the impact of cache configurations on memory access latency.

## Conclusion
This project provides a comprehensive understanding of cache mechanisms and their significance in computer architecture. By simulating different cache configurations and analyzing their impact on performance, the project highlights the importance of tailoring cache designs to workload characteristics for optimal system performance.

## Usage
To use the cache simulator:
1. Define the cache configuration parameters (size, associativity, replacement/write policy).
2. Load the trace file corresponding to the workload.
3. Simulate the cache behavior by processing the trace file through the simulator.
4. Record detailed results for the defined performance metrics.
5. Repeat the process for all combinations of configurations and workloads.

## Dependencies
- C++ compiler
- Trace files for workloads
