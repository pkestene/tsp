# How to build clang++ with openmp target (off-loading) support

See the following links:
- https://devmesh.intel.com/blog/724749/how-to-build-and-run-your-modern-parallel-code-in-c-17-and-openmp-4-5-library-on-nvidia-gpus
- https://hpc-wiki.info/hpc/Building_LLVM/Clang_with_OpenMP_Offloading_to_NVIDIA_GPUs


## Get llvm sources

As of today (December 2020), the highest cuda version compatible with clang, is cuda 10.1; as such we use llvm/clang 10.x release branch.

```shell
git clone git@github.com:llvm/llvm-project.git
cd llvm-project
git checkout -b release/10.x origin/release/10.x
```

I also tried to build llvm from release branch 11.x, with cuda/11.2; it basically works.

## First Build with gcc

Make sure to use g++ version 8.x (the highest one compatible with cuda 10.2).

```shell
module load cuda/10.2

mkdir build_llvm_10.x; cd build_llvm_10.x

ccmake  -DCLANG_OPENMP_NVPTX_DEFAULT_ARCH=sm_75 -DLIBOMPTARGET_NVPTX_COMPUTE_CAPABILITIES=37,60,70,75 -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;libcxx;libcxxabi;lld;openmp" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/data/pkestene/local/llvm-10-omptarget -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ../llvm
make -j 4
make install
```

## Second build with clang

```shell
module load cuda/10.2

mkdir build_llvm_10.x-openmp; cd build_llvm_10.x-openmp

ccmake  -DCLANG_OPENMP_NVPTX_DEFAULT_ARCH=sm_75 -DLIBOMPTARGET_NVPTX_COMPUTE_CAPABILITIES=37,60,70,75 -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;libcxx;libcxxabi;lld;openmp" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/data/pkestene/local/llvm-10-omptarget -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../openmp
make -j 4
make install
```
