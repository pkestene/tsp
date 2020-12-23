tsp_pgi_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	#nvc++ -fast -stdpar=multicore -target=multicore -tp=native -std=c++17 -Wall $< -o $@
	nvc++ -w -fast -Mnouniform -Mfprelaxed -stdpar=multicore -std=c++17 $< -o $@ -ltbb


tsp_pgi_stdpar_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -fast -stdpar=gpu -Mnouniform -Mfprelaxed -std=c++11 $< -o $@

all: tsp_pgi_stdpar_cpu

clean:
	\rm -f tsp_pgi_stdpar_cpu tsp_pgi_stdpar_gpu
