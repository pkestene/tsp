# Note

## GNU

For GNU g++ (version 9 or 10), stdpar relies on a old TBB version, so:
- don't use Intel OenAPI/tbb, it is not compatible with g++/stdpar implementation
- just use an old version of tbb: on ubuntu 20.04, `sudo apt instal libtbb-dev`

see https://community.intel.com/t5/Intel-oneAPI-Threading-Building/tbb-task-has-not-been-declared/m-p/1255723?profile.language=zh-TW&countrylabel=Mexico

##  NVIDIA compiler (nvhpc) CPU and GPU

For the CPU version, you can use OneAPI TBB implementation

```shell
module load tbb/latest
```

## Intel OneAPI dpcpp compiler

You can of course use OneAPI TBB implementation:

```shell
module load compiler/latest tbb/latest
```

on this particular example, dpcpp provides a much faster executable than nvc++/cpu