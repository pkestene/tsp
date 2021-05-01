# How to build kokkos version

- provide path to where Kokkos source where cloned, e.g.
  ```shell
  export KOKKOS_PATH=/home/kestener/install/kokkos/git/kokkos
  ```
- build for OpenMP backend:
  ```shell
  make -j 8 CUDA_ARCH="Zen" CUDA_DEVICES="OpenMP"
  ```
- build for Cuda backend:
  ```shell
  make -j 8 CUDA_ARCH="Zen,Ampere80" CUDA_DEVICES="OpenMP,Cuda"
  ```