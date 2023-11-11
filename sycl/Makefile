# =========================
# Intel OneAPI dpcpp (CPU)
# =========================
# module use /opt/intel/oneapi/modulefiles
# module load compiler/2023.2.1
tsp_sycl_dpcpp_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	dpcpp -std=c++17 -fsycl -O3 -fsycl-unnamed-lambda $< -o $@

# ======================================================
# Intel OneAPI LLVM rebuilt with for Nvidia GPUs support
# ======================================================
# module use /opt/intel/oneapi/modulefiles
# module load compiler/2023.2.1
# download OneAPI for Nvidia GPUs from https://developer.codeplay.com/products/oneapi/nvidia/home/
# sudo sh oneapi-for-nvidia-gpus-2023.2.1-cuda-12.0-linux.sh
# this will install clang++
# to make it available, just update PATH
# export PATH=/opt/intel/oneapi/compiler/2023.2.1/linux/bin-llvm:$PATH
tsp_sycl_llvm_intel_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	clang++ -O2 -g -std=c++17 -fsycl -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -fsycl-unnamed-lambda $< -o $@

all: tsp_sycl_dpcpp_cpu
