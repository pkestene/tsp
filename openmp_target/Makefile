tsp_clang_openmp_target: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	clang++ -include __clang_cuda_runtime_wrapper.h -std=c++17 -stdlib=libc++ -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -Wall -O3 $< -o $@

all: tsp_clang_openmp_target
