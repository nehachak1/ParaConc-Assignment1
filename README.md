# OpenMP Parallel Computing: Performance and Optimization

## Overview

This assignment explores the implementation and optimization of parallel algorithms using OpenMP. The objective is to evaluate the impact of parallelization on performance, scalability, and resource utilization in shared-memory systems.

## Programs

### Program A – Monte Carlo Estimation of π
- Parallelized Monte Carlo simulation for estimating π.
- Analysis of execution time and speedup across multiple thread counts.
- Comparison between predicted and measured scalability.

### Program B – Monte Carlo Numerical Integration
- Parallel numerical integration using the Monte Carlo method.
- Evaluation of performance improvements through multithreading.
- Analysis of overheads affecting scalability.

### Program C – True/False Sharing Optimization
- Investigation of cache contention issues in parallel programs.
- Implementation of thread-local histograms to reduce false sharing.
- Performance comparison before and after optimization.

### Program D – Parallel Matrix Multiplication Optimization
- OpenMP-based optimization of matrix multiplication.
- Techniques include loop parallelization, cache optimization, loop tiling, and loop unrolling.
- Performance benchmarking on different thread configurations.

## Technologies Used

- C Programming Language
- OpenMP
- SCITAS Cluster
- Performance Benchmarking Tools

## Key Concepts

- Parallelism and Concurrency
- Shared-Memory Programming
- OpenMP Directives
- Speedup and Scalability
- Cache Locality
- False Sharing
- Loop Optimization
- High-Performance Computing (HPC)

## Results

The experiments demonstrate that parallelization significantly improves execution time for computationally intensive tasks. While near-linear speedup is achieved for many workloads, hardware limitations, synchronization overhead, and memory contention become significant factors at higher thread counts.

## Authors

- Neha Chakraborty
- Guillaume Marie Lepin
