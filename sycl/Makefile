# =======================
# CodePlay
# =======================
# module load computecpp/2.5.0
#
# currently, not working, TODO
#
# OPENCL_INC=/usr/local/cuda-11.3/targets/x86_64-linux/include
# OPENCL_LIB=/usr/local/cuda-11.3/targets/x86_64-linux/include
# tsp_sycl_computecpp_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
# 	compute++ -sycl -O2 -mllvm -inline-threshold=1000 -intelspirmetadata -DSYCL_LANGUAGE_VERSION=2017 -sycl-target ptx64 -std=c++14 -I$(OPENCL_INC) -I$(COMPUTECPP_INC_DIR) -sycl-ih tsp.cpp.computecpp.sycl -o tsp.cpp.computecpp.s -c $<
# 	g++ tsp.cpp.o-o $@ -L$(COMPUTECPP_LIB_DIR) -lComputeCpp $(OPENCL_LIB)

# =========================
# Intel OneAPI dpcpp (CPU)
# =========================
# module use /opt/intel/oneapi/modulefiles
# module load compiler/2021.2.0
tsp_sycl_dpcpp_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	dpcpp -std=c++17 -fsycl -O3 -fsycl-unnamed-lambda $< -o $@ -L${ONEAPI_ROOT}/tbb/2021.1.1/lib/intel64/gcc4.8 -ltbb

# ======================================================
# Intel OneAPI LLVM rebuilt with for Nvidia GPUs support
# ======================================================
# module load llvm/12-intel-sycl-cuda
tsp_sycl_llvm_intel_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	clang++ -O2 -g -std=c++17 -fsycl -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -fsycl-unnamed-lambda -I/data/pkestene/local/intel/oneapi/dev-utilities/2021.1.1/include $< -o $@

all: tsp_sycl_dpcpp_cpu
