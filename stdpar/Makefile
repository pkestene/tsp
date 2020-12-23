tsp_pgi_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	nvc++ -fast -stdpar=multicore -target=multicore -tp=native -I/usr/local/cuda/include -std=c++17 -Wall $< -o $@

tsp_pgi_stdpar_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	nvc++ -fast -stdpar=gpu -I/usr/local/cuda/include -std=c++17 -Wall  $< -o $@

all: tsp_pgi_stdpar_cpu

clean:
	\rm -f tsp_pgi_stdpar_cpu tsp_pgi_stdpar_gpu
