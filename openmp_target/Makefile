tsp_clang_openmp_target: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	clang++  -std=c++17 -O3 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -stdlib=libc++ -Xopenmp-target -march=sm_75 $< -o $@

all: tsp_clang_openmp_target

clean:
	rm -f tsp_clang_openmp_target
