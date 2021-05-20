# module load llvm/12-omptarget
tsp_clang_openmp_target: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	clang++  -std=c++17 -O3 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -stdlib=libc++ -Xopenmp-target -march=sm_80 $< -o $@

# May 2021 : unable to make gcc compile this code
#tsp_gnu_openmp_target: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
#	g++  -std=c++17 -O3 -fopenmp -fcf-protection=none -foffload="-lm" -lm  $< -o $@

# nvc++ (21.3) does not currently support user defined reduction
# we'll have to wait for now
#tsp_nvidia_openmp_target: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
#	nvc++ --c++17 -mp=gpu -Minfo=accel -fast -ta=tesla:cc75 $< -o $@


all: tsp_clang_openmp_target

clean:
	rm -f tsp_clang_openmp_target
