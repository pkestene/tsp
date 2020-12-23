#ifndef ROUTE_COST_H_
#define ROUTE_COST_H_

#include <cstdint>
#include <limits>

// ============================================
// ============================================
struct route_cost
{
  int64_t route;
  int cost;

  route_cost()
    : route(-1),
      cost(std::numeric_limits<int>::max())
  { }

  route_cost(int64_t route, int cost)
    : route(route),
      cost(cost)
  { }

  static struct min_functor
  {
    route_cost operator()(route_cost& x, route_cost& y) const
    {
      return x.cost < y.cost ? x : y;
    }
  } minf;

  static
  route_cost min(const route_cost& x, const route_cost& y)
  {
    if (x.cost < y.cost)
    {
      return x;
    }
    return y;
  }

}; // route_cost

#endif // ROUTE_COST_H_
