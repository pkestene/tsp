Solving the Traveling Salesman Problem (TSP) with brute force and
parallelism just for teaching and illustrative purpose.

# [Traveling Salesman Problem](https://en.wikipedia.org/wiki/Travelling_salesman_problem)

# Brute force parallelism to solve TSP

The code here is mostly adapted from the very nice talk by David Olsen, [Faster Code Through Parallelism on CPUs and GPUs](https://www.youtube.com/watch?v=cbbKEAWf1ow) at [CppCon 2019](https://cppcon.org/cppcon-2019-program/).

I've just slightly changed the way the permutations are computed which turns out to be slightly faster than the original.

See also companion slides by D. Olsen (Nvidia) at GTC2019 [s9770-c++17-parallel-algorithms-for-nvidia-gpus-with-pgi-c++.pdf](https://developer.download.nvidia.com/video/gputechconf/gtc/2019/presentation/s9770-c++17-parallel-algorithms-for-nvidia-gpus-with-pgi-c++.pdf)

# Timings

Timings reported here are measured on my own desktop.

Hardware :
- CPU Intel(R) Core(TM) i5-9400F @ 2.90 GHz - 6 cores
- GPU Nvidia GeForce RTX 2060

Software:
- Ubuntu 20.04
- [Nvidia hpc sdk version 20.11](https://developer.nvidia.com/hpc-sdk) with cuda 11.2
- g++ 9.3

## serial version (reference)

Number of cities  | time (seconds)
----------------- | ---------------
10                |    0.064
11                |    0.66
12                |    8.48
13                |  130.7
14                | 1997.9

## OpenMP pragmas

### GNU compiler

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.044
11                |   0.152
12                |   1.64
13                |  24.2
14                | 368.3

## OpenMP target for Nvidia

We need an OpenMP 4.5 capable compiler:

Just follow the steps mentioned here:
- [how-to-build-and-run-your-modern-parallel-code-in-c-17-and-openmp-4-5-library-on-nvidia-gpus](https://devmesh.intel.com/blog/724749/how-to-build-and-run-your-modern-parallel-code-in-c-17-and-openmp-4-5-library-on-nvidia-gpus)
- [Clang_with_OpenMP_Offloading_to_NVIDIA_GPUs](https://hpc-wiki.info/hpc/Building_LLVM/Clang_with_OpenMP_Offloading_to_NVIDIA_GPUs)

Performance are quite slow (compared to PGI/OpenAcc or Kokkos::CUDA below) by a factor of x3 when N >= 12.

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.077
11                |   0.104
12                |   0.39
13                |   3.56
14                |  59.5

I tried three compile toolchains:
- clang 10.0 with cuda 10.1
- clang 11.0 with cuda 11.2
- clang 12.0 with cuda 11.3

## OpenAcc (PGI compiler)

### OpenAcc for multicore CPU

These results are strange, much too slow (maybe related to my host configuration ?). I also a strong performance drop when using  nvhpc 20.11 versus 20.5 (??).

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.030
11                |   0.25
12                |   7.21
13                |  XX
14                |  XX

### OpenAcc for GPU

Performance looks good (similar to Kokkos::CUDA below).

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.107
11                |   0.128
12                |   0.25
13                |   1.51
14                |  20.5

## Kokkos

To build, you just need to edit `kokkos/Makefile`, and change the first line by modifying variable `KOKKOS_PATH` to the full path where you cloned [kokkos](https://github.com/kokkos/kokkos/) sources.

### Kokkos - OpenMP

Just run `make` in subdir `kokkos`.

Timings are similar to OpenMP pragma.

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.015
11                |   0.152
12                |   1.84
13                |  24.5
14                | 376.6

### Kokkos - Cuda

Just run `make KOKKOS_DEVICES=Cuda` in subdir `kokkos`.

Timings are very similar to Openacc (GPU).

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.01
11                |   0.01
12                |   0.106
13                |   1.41
14                |  22.4

## stdpar

### stdpar for multicore cpu (nvc++ compiler)

Again, performance obtained here is odd; it's much slower than OpenMP with g++, but should be similar....

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.027
11                |   0.128
12                |   5.10
13                | 114.0
14                |  TODO

### stdpar for GPU (nvc++ compiler)

Similar performance as OpenAcc and Kokkos/Cuda.

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.007
11                |   0.016
12                |   0.15
13                |   1.40
14                |  21.3

## SYCL

There are a lot of SYCL implementation available, here we tried
- Intel OneAPI DPC++ (linux) for x86 multi-core
- Intel LLVM (linux) for Nvidia GPU (see https://github.com/intel/llvm)

### SYCL OneAPI for x86 multicore

`module load compiler/2021.1.1`

Number of cities  | time (seconds)
----------------- | ---------------
10                |   1.29
11                |   1.40
12                |   2.62
13                |  21.3
14                |  TODO

for large values of N, we retrieve the expected results (OpenMP, Kokkos::OpenMP, ...).

### SYCL OneAPI for Nvidia GPU (LLVM/intel)

`module load llvm/12-intel-sycl-cuda`

These results should be optimized by changing the number of block of threads and the block size (à la CUDA).

Performance are correct, maybe a bit slow for N=14.

Number of cities  | time (seconds)
----------------- | ---------------
10                |   0.001
11                |   0.008
12                |   0.11
13                |   1.86
14                |  37.4
