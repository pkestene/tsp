#
# GNU g++ by means of TBB
#
ifeq ($(TBBROOT),)
	TBB_INC=/usr/include
	TBB_LIB=/usr/lib/x86_64-linux-gnu
else
	TBB_INC=$(TBBROOT)/include/oneapi
	TBB_LIB=$(TBBROOT)/lib/intel64/gcc4.8
endif
tsp_gnu_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	g++ -O3 -std=c++17 -I$(TBB_INC) $< -o $@ -ltbb

#
# NVIDIA/PGI for CPU (via tbb) and for GPU
#
tsp_pgi_stdpar_cpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -w -fast -Mipa=fast -Mnouniform -Mfprelaxed -stdpar=multicore -std=c++17 $< -o $@ -L${TBB_LIB} -ltbb

tsp_pgi_stdpar_gpu: tsp.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	nvc++ -fast -stdpar=gpu -Mnouniform -Mfprelaxed -std=c++11 $< -o $@

#
# Intel OneAPI using TBB under the hood
#
tsp_dpcpp_stdpar_cpu: tsp_oneapi.cpp tsp_utils.h route_cost.h route_iterator.h counting_iterator.h
	dpcpp -O3 -std=c++17 -ferror-limit=3 $< -o $@ -L${TBB_LIB} -ltbb 

all: tsp_gnu_stdpar_cpu tsp_pgi_stdpar_cpu

clean:
	\rm -f tsp_pgi_stdpar_cpu tsp_pgi_stdpar_gpu
	\rm -f tsp_dpcpp_stdpar_cpu
