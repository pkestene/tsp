#include "route_iterator.h"
// #include "route_cost.h" // we don't need it for Kokkos
#include "tsp_utils.h"

#include "SimpleTimer.h"

#include <Kokkos_Core.hpp>
#include <typeinfo>

#include <cstdio> // for printf
#include <cstdlib> // for EXIT_SUCCESS
#include <iostream>

using reducer_t = Kokkos::MinLoc<int, int64_t, Kokkos::HostSpace>;
using route_cost = reducer_t::value_type;


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
route_cost find_best_route(int const* distances_raw)
{

  Kokkos::View<int*> distances("distances", N*N);
  Kokkos::deep_copy(distances,
    Kokkos::View<const int*, Kokkos::HostSpace>(distances_raw, N*N));

  // brut force, total number of routes
  int64_t num_routes = factorial(N);

  // initialize minimal cost value
  route_cost best_route;

  Kokkos::parallel_reduce("FindBestRoute",
                          Kokkos::RangePolicy<Kokkos::IndexType<int64_t>>(0,num_routes),
                          KOKKOS_LAMBDA(const int64_t& i,
                                        route_cost& rcost)
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
      cost += distances[to + N*from];
      from = to;
    }

    // debug
    // printf("route #%d cost=%d\n",i,cost);

    // update best_route -> reduction
    //rcost = cost > rcost.val ? rcost : route_cost(cost, i);
    if (cost < rcost.val)
    {
      rcost.val = cost;
      rcost.loc = i;
    }
  },
  reducer_t(best_route));

  return best_route;

} // find_best_route

// ============================================================
// ============================================================
//! \param[in] nbRepeat number of repeat (for accurate time measurement)
template <int N>
void solve_traveling_salesman(int nbRepeat = 1)
{
  // find best route
  auto distances_small = init_distance_matrix_small(N);

  route_cost best_route;

  SimpleTimer timer;
  timer.start();
  for (int i = 0; i<nbRepeat; ++i)
    best_route = find_best_route<N>(distances_small.data());
  timer.stop();

  double time = timer.elapsedSeconds()/nbRepeat;

  // print best route
  printf("Trav Salesman Prob N=%d, best route cost is: %d, average time is %f seconds\n",N, best_route.val, time);

  printf("Solution route is ");
  route_iterator<N> rit(best_route.loc);
  rit.print();

} // solve_traveling_salesman

// ============================================================
// ============================================================
int main(int argc, char* argv[])
{

  Kokkos::initialize(argc, argv);

  std::cout << "##########################\n";
  std::cout << "KOKKOS CONFIG             \n";
  std::cout << "##########################\n";

  std::ostringstream msg;
  std::cout << "Kokkos configuration" << std::endl;
  if ( Kokkos::hwloc::available() ) {
    msg << "hwloc( NUMA[" << Kokkos::hwloc::get_available_numa_count()
    << "] x CORE["    << Kokkos::hwloc::get_available_cores_per_numa()
    << "] x HT["      << Kokkos::hwloc::get_available_threads_per_core()
    << "] )"
    << std::endl ;
  }
  Kokkos::print_configuration( msg );
  std::cout << msg.str();
  std::cout << "##########################\n";


  printf("Running on Kokkos execution space %s\n",
         typeid(Kokkos::DefaultExecutionSpace).name());


  // print factorials up to 14!
  // print_factorials(14);

  // dump permutations on screen
  // constexpr int N=4;
  // test_permutation<N>();

  // auto cityIndex = makeCityMap();
  // for (auto city : cityIndex)
  //   std::cout << city.first << " " << city.second << "\n";

  //test_city_distance();

  int n = 8;
  if (argc>1)
    n = atoi(argv[1]);

  if (n==4)
      solve_traveling_salesman<4>(10000);
  else if (n==5)
      solve_traveling_salesman<5>(10000);
  else if (n==6)
      solve_traveling_salesman<6>(1000);
  else if (n==7)
    solve_traveling_salesman<7>(1000);
  else if (n==8)
    solve_traveling_salesman<8>(100);
  else if (n==9)
    solve_traveling_salesman<9>(10);
  else if (n==10)
    solve_traveling_salesman<10>();
  else if (n==11)
    solve_traveling_salesman<11>();
  else if (n==12)
    solve_traveling_salesman<12>();
  else if (n==13)
    solve_traveling_salesman<13>();
  else if (n==14)
    solve_traveling_salesman<14>();

  Kokkos::finalize();

  return EXIT_SUCCESS;

} // main
