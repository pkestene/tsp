#include "route_iterator.h"
//#include "route_iterator_orig.h"
#include "route_cost.h"
#include "tsp_utils.h"

#include "SimpleTimer.h"

#include <cstdio> // for printf
#include <cstdlib> // for EXIT_SUCCESS
#include <iostream>

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
    //route_iterator<N>::print(k);

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
route_cost find_best_route(int const* distances)
{

  // brut force, total number of routes
  int64_t num_routes = factorial(N);


  int num_blocks = 1024;
  int block_size = 128;
  int threads = num_blocks * block_size;

  // intermediate array of partial reduction results
  route_cost* best_thread = new route_cost[threads];

  // open a parallel region
  // copy input data to device
#pragma acc enter data \
  copyin(best_thread[0:threads]) \
  copyin(distances[0:N*N])
#pragma acc parallel \
  num_gangs(num_blocks) \
  vector_length(block_size) \
  present(best_thread, distances)
#pragma acc loop gang
  for (int ig = 0; ig < num_blocks; ++ig)
  {
#pragma acc loop vector
    for (int iv = 0; iv < block_size; ++iv)
    {
      route_cost best_route;
      int idx = ig * block_size + iv;

#pragma acc loop seq
      for (int64_t i = idx; i < num_routes; i += threads)
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

        // update best_route -> reduction
        best_route = route_cost::min(best_route, route_cost(i, cost));
      } // for i

      best_thread[idx] = best_route;

    } // for iv
  } // for ig

#pragma acc update self(best_thread[:threads])
  route_cost best_route;

  // last reduction stage on host
  for (int64_t i = 0; i < threads; ++i)
  {
    best_route = route_cost::min(best_route, best_thread[i]);
  }

#pragma acc exit data delete(best_thread,distances)

  delete[] best_thread;

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
  printf("Trav Salesman Prob N=%d, best route cost is: %d, average time is %f seconds\n",N, best_route.cost, time);

  printf("Solution route is ");
  route_iterator<N> rit(best_route.route);
  rit.print();
  //route_iterator<N>::print(best_route.route);

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

  return EXIT_SUCCESS;

} // main
