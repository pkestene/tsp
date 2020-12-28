tsp_gnu_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	g++ -O3 -std=c++17 $< -o $@ -ltbb

tsp_pgi_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -w -fast -Mipa=fast -Mnouniform -Mfprelaxed -stdpar=multicore -std=c++17 $< -o $@ -ltbb

tsp_pgi_stdpar_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -fast -stdpar=gpu -Mnouniform -Mfprelaxed -std=c++11 $< -o $@

all: tsp_gnu_stdpar_cpu tsp_pgi_stdpar_cpu

clean:
	\rm -f tsp_pgi_stdpar_cpu tsp_pgi_stdpar_gpu
