# CodePlay
#tsp_sycl_computecpp_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
#	compute++ -O2 -std=c++17 -sycl -fsycl-unnamed-lambda -ferror-limit=3 -sycl-driver -no-serial-memop -I$(COMPUTECPP_INC_DIR) $< -L$(COMPUTECPP_LIB_DIR) -lComputeCpp -o $@

# Intel OneAPI dpcpp (CPU)
tsp_sycl_dpcpp_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	dpcpp -std=c++17 -fsycl -O3 -fsycl-unnamed-lambda $< -o $@ -L${ONEAPI_ROOT}/tbb/2021.1.1/lib/intel64/gcc4.8 -ltbb

# Intel LLVM for Nvidia GPUs
tsp_sycl_llvm_intel_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	clang++ -O2 -g -std=c++17 -fsycl -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -fsycl-unnamed-lambda -I/data/pkestene/local/intel/oneapi/dev-utilities/2021.1.1/include $< -o $@

all: tsp_sycl_dpcpp_cpu
