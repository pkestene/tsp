#
# GNU g++ by means of TBB
#
tsp_gnu_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	g++ -O3 -std=c++17 $< -o $@ -ltbb

#
# NVIDIA/PGI for CPU (via tbb) and for GPU
#
tsp_pgi_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -w -fast -Mipa=fast -Mnouniform -Mfprelaxed -stdpar=multicore -std=c++17 $< -o $@ -ltbb

tsp_pgi_stdpar_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -fast -stdpar=gpu -Mnouniform -Mfprelaxed -std=c++11 $< -o $@

#
# Intel OneAPI using TBB under the hood
#
tsp_dpcpp_stdpar_cpu: tsp_oneapi.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	dpcpp -O3 -fsycl -std=c++17 -ferror-limit=3 -fsycl-unnamed-lambda $< -o $@ -L${ONEAPI_ROOT}/tbb/2021.1.1/lib/intel64/gcc4.8 -ltbb 

all: tsp_gnu_stdpar_cpu tsp_pgi_stdpar_cpu

clean:
	\rm -f tsp_pgi_stdpar_cpu tsp_pgi_stdpar_gpu
	\rm -f tsp_dpcpp_stdpar_cpu
