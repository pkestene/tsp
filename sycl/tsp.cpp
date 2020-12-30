#include "route_iterator.h"
#include "route_cost.h"
#include "tsp_utils.h"

#include "SimpleTimer.h"

#include <cstdio> // for printf
#include <cstdlib> // for EXIT_SUCCESS
#include <iostream>

// SYCL
#include "CL/sycl.hpp"

namespace sycl = cl::sycl;

// ============================================
// ============================================
void print_factorials(int N)
{
  for (int k=0; k<N; ++k)
    printf("factorial(%d)=%" PRId64 "\n",k,factorial(k));
}

// ============================================
// ============================================
template<int N>
void test_permutation()
{

  for (int k=0; k<factorial(N); ++k)
  {

    // create permutation
    route_iterator<N> rit(k);

    // print on screen for cross-check
    rit.print();

  }

} // test_permutation

// ============================================
// ============================================
void test_city_distance()
{
  auto cityIndex = makeCityMap();
  auto size = cityIndex.size();

  auto distances = init_distance_matrix();

  for (size_t from=0; from<size; ++from)
  {
    for (size_t to=0; to<size; ++to)
    {
      assert(distances[to+size*from] == distances[from+size*to]);
      printf("%05d ",distances[to+size*from]);
    }
    printf("\n");
  }

  int small_n = 5;
  auto distances_small = init_distance_matrix_small(small_n);

  for (int from=0; from<small_n; ++from)
  {
    for (int to=0; to<small_n; ++to)
    {
      assert(distances_small[to+small_n*from] == distances_small[from+small_n*to]);
      printf("%05d ",distances_small[to+small_n*from]);
    }
    printf("\n");
  }

} // test_city_distance

// ============================================
// ============================================
template<int N>
route_cost find_best_route(sycl::queue& q, int const* distances)
{

  auto device = q.get_device();

  // NDRange configuration for the reduction kernel
  size_t num_groups = device.is_cpu() ?
    device.get_info<sycl::info::device::max_compute_units>() :
    device.get_info<sycl::info::device::max_compute_units>() * 4;

  size_t wgsize = device.is_cpu() ?
    1 : /*device.get_info<sycl::info::device::native_vector_width_double>() * 2 :*/
    device.get_info<sycl::info::device::max_work_group_size>();

  std::cout << "num_groups = " << num_groups << "\n";
  std::cout << "wgsize     = " << wgsize << "\n";

  // data handler for sycl runtime
  sycl::buffer<int, 1> distances_buf{distances, sycl::range<1>{N*N}};

  // do not copy back buffer after kernel computation
  distances_buf.set_final_data(nullptr);

  std::vector<route_cost> part_res(num_groups, route_cost());

  // partial best route computation, one per working group
  sycl::buffer<route_cost, 1> part_res_buf{part_res.data(), sycl::range<1>{num_groups}};

  // we adopt the two steps strategies:
  // 1. compute partial reduction on device
  // 2. final reduction on host

  // step 1 : on device
  q.submit(
    [&](sycl::handler& h)
    {

      // brut force, total number of routes
      int64_t num_routes = factorial(N);

      // accessor to distances buffer
      auto distances_d =
        distances_buf.template get_access<sycl::access::mode::read>(h);

      auto part_res_d =
        part_res_buf.template get_access<sycl::access::mode::write>(h);

      // scratch (= shared memory in cuda) array
      auto wg_res = sycl::accessor<route_cost, 1,
                                   sycl::access::mode::read_write,
                                   sycl::access::target::local>(sycl::range<1>(wgsize), h);

      sycl::nd_range<1> reduce_range{wgsize*num_groups,  // global size
                                     wgsize};            // local  size

      h.parallel_for(reduce_range,
                     [=](sycl::nd_item<1> id)
       {

         size_t i = id.get_global_id(0);
         size_t li = id.get_local_id(0);
         size_t global_size = id.get_global_range()[0];

         // each work item perform its own reduction
         wg_res[li].route = -1;
         wg_res[li].cost = std::numeric_limits<int>::max();

         for (; i < num_routes; i += global_size)
         {
           int cost = 0;

           route_iterator<N> it(i);

           // first city visited
           int from = it.first();

           // visited all other cities in the chosen route
           // and compute cost
           while (!it.done())
           {
             int to = it.next();
             cost += distances_d[to + N*from];
             from = to;
           }

           // update best_route -> reduction
           wg_res[li] = route_cost::min(wg_res[li], route_cost(i, cost));

         } // end for i

         // perform intra work group reduction, until we have only one result per workgroup
         size_t local_size = id.get_local_range()[0];
         for (int offset = local_size / 2; offset > 0; offset /= 2)
         {
           id.barrier(cl::sycl::access::fence_space::local_space);
           if (li < offset)
           {
             wg_res[li] = route_cost::min(wg_res[li], wg_res[li + offset]);
           }
         }

         // only the first thread write the final result in global memory
         if (li == 0)
           part_res_d[id.get_group(0)] = wg_res[0];

       }); // end of parallel_for
    }); // end q.submit

  // step 2 : on host

  route_cost best_route;

  auto part_res_h = part_res_buf.template get_access<sycl::access::mode::read>();
  for (int i = 0; i < num_groups; ++i)
  {
    best_route = route_cost::min(best_route, part_res_h[i]);
  }

  return best_route;

} // find_best_route

// ============================================================
// ============================================================
//! \param[in] nbRepeat number of repeat (for accurate time measurement)
template <int N>
void solve_traveling_salesman(int nbRepeat = 1)
{

  // create a sycl queue
  sycl::queue q([=](sycl::exception_list eL) {
      try {
        for (auto& e : eL) {
          std::rethrow_exception(e);
        }
      } catch (sycl::exception ex) {
        std::cout << " There is an exception in the reduction kernel"
                  << std::endl;
        std::cout << ex.what() << std::endl;
      }
    });

  auto device = q.get_device();
  auto deviceName = device.get_info<sycl::info::device::name>();
  std::cout << " Device Name: " << deviceName << std::endl;
  auto platformName =
    device.get_platform().get_info<sycl::info::platform::name>();
  std::cout << " Platform Name " << platformName << std::endl;

  // find best route
  auto distances_small = init_distance_matrix_small(N);

  route_cost best_route;

  SimpleTimer timer;
  timer.start();
  for (int i = 0; i<nbRepeat; ++i)
    best_route = find_best_route<N>(q, distances_small.data());
  timer.stop();

  double time = timer.elapsedSeconds()/nbRepeat;

  // print best route
  printf("Trav Salesman Prob N=%d, best route cost is: %d, average time is %f seconds\n",N, best_route.cost, time);

  printf("Solution route is ");
  route_iterator<N> rit(best_route.route);
  rit.print();

} // solve_traveling_salesman

// ============================================================
// ============================================================
int main(int argc, char* argv[])
{

  // print factorials up to 14!
  // print_factorials(14);

  // dump permutations on screen
  // constexpr int N=4;
  // test_permutation<N>();

  // auto cityIndex = makeCityMap();
  // for (auto city : cityIndex)
  //   std::cout << city.first << " " << city.second << "\n";

  //test_city_distance();

  int n = 10;
  if (argc>1)
    n = atoi(argv[1]);

  if (n==10)
    solve_traveling_salesman<10>(10);
  else if (n==11)
    solve_traveling_salesman<11>(10);
  else if (n==12)
    solve_traveling_salesman<12>(5);
  else if (n==13)
    solve_traveling_salesman<13>(3);
  else if (n==14)
    solve_traveling_salesman<14>();

  return EXIT_SUCCESS;

} // main
