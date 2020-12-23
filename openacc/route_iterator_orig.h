#ifndef ROUTE_ITERATOR_H_
#define ROUTE_ITERATOR_H_

#include <cstdint>
#include <cstdio>
#include <inttypes.h>
#include <cassert>

// =====================================================
// =====================================================
//! function to find factorial of given number
uint64_t factorial(uint64_t n)
{
    if (n == 0)
      return 1;
    return n * factorial(n - 1);
}

// =====================================================
// =====================================================
//! integer division, compute quotient and reminder
inline void int_div(uint64_t k, uint64_t divisor,
                    uint64_t& q, uint64_t& r)
{
  q = k / divisor;
  r = k - divisor*q;
}

// =====================================================
// =====================================================
//! swap values
template <typename T>
void my_swap(T& a, T& b)
{
    T c(a); a=b; b=c;
}

// =====================================================
// =====================================================
/**
 * Generate a unique permutation of N elements.
 */
template <int N>
class route_iterator
{
public:
  //! constructor
  route_iterator(uint64_t route_id);

  //! at the end of the route ?
  bool done() const { return m_hops_left <= 0; };

  //! first city of the route
  int first()
  {
    int index = (int) (m_remainder % m_hops_left);
    m_remainder /= m_hops_left;
    --m_hops_left;
    m_visited = (1 << index);
    return index;
  };

  //! next city of the route
  int next()
  {
    int64_t available = m_remainder % m_hops_left;
    m_remainder /= m_hops_left;
    --m_hops_left;
    int index = 0;
    while (true)
    {
      if ( (m_visited & (1 << index)) == 0)
      {
        if (--available < 0)
        {
          break;
        }
      }
      ++index;
    }
    m_visited |= (1 << index);

    return index;
  };

  static void print(uint64_t route_id);

private:
  int64_t m_remainder;
  int m_hops_left;
  uint32_t m_visited;

}; // class route_iterator

// =====================================================
// =====================================================
template<int N>
route_iterator<N>::route_iterator(uint64_t route_id)
  : m_remainder(route_id),
    m_hops_left(N)
{
  assert(m_remainder < factorial(N));
}

// =====================================================
// =====================================================
template<int N>
void route_iterator<N>::print(uint64_t route_id)
{
  printf("Permutation #%06" PRId64 " || ", route_id);

  route_iterator rit(route_id);

  printf("%2d ",rit.first());
  while(!rit.done())
  {
    printf("%2d ",rit.next());
  }
  printf("\n");

} // route_iterator::print

#endif // ROUTE_ITERATOR_H_
