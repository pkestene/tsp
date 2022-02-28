# How to build kokkos version

- provide path to where Kokkos source where cloned, e.g.
  ```shell
  export KOKKOS_PATH=/home/kestener/install/kokkos/git/kokkos
  ```
- build for OpenMP backend:
  ```shell
  make -j 8 KOKKOS_ARCH="Zen" KOKKOS_DEVICES="OpenMP"
  ```
- build for Cuda backend:
  ```shell
  module load cuda/11.5
  make -j 8 KOKKOS_ARCH="Zen,Ampere80" KOKKOS_DEVICES="OpenMP,Cuda"
  ```
