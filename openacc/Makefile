tsp_pgi_openacc_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	nvc++ -fast -O3 -ta=multicore -tp=native $< -o $@

tsp_pgi_openacc_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h
	nvc++ -Minfo=accel -fast -acc -ta=tesla:cc80 $< -o $@

all: tsp_pgi_openacc_cpu tsp_pgi_openacc_gpu

clean:
	\rm -rf tsp_pgi_openacc_cpu tsp_pgi_openacc_gpu
