tsp_gnu_openmp: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	g++ -std=c++17 -Wall -march=native -O3 -fopenmp $< -o $@

#tsp_pgi_openmp: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
#	pgc++ --c++17 -fast -tp=skylake -mp $< -o $@

all: tsp_gnu_openmp
