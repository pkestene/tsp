# About SYCL compiler for linux

## x86

Just use [Intel OneAPI package](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html)

## Nvidia GPUs

As Intel OneAPI package does not contain CUDA support, you need to recompile from source [llvm/intel](https://github.com/intel/llvm) by following [these instructions](https://intel.github.io/llvm-docs/GetStartedGuide.html#build-dpc-toolchain-with-support-for-nvidia-cuda)


