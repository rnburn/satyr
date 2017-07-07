#pragma once

#include <satyr/linear_algebra/concept.h>
#include <cmath>

namespace satyr {
//------------------------------------------------------------------------------
// norm2_squared
//------------------------------------------------------------------------------
template <Vector X>
  requires RealScalar<value_type_t<X>>
value_type_t<X> norm2_squared(const X& x) {
  value_type_t<X> result = 0;
  for_each(x, [&] (value_type_t<X> element) {
      result += element*element;
  });
  return result;
}

//------------------------------------------------------------------------------
// norm2
//------------------------------------------------------------------------------
template <Vector X>
  requires RealScalar<value_type_t<X>>
value_type_t<X> norm2(const X& x) {
  return std::sqrt(norm2_squared(x));
}

//------------------------------------------------------------------------------
// norm1
//------------------------------------------------------------------------------
template <Matrix A>
  requires RealScalar<value_type_t<A>>
value_type_t<A> norm1(const A& a) {
  value_type_t<A> result = 0;
  auto[m, n] = a.shape();
  for (index_t j = 0; j < n; ++j) {
    value_type_t<A> abs_column_sum = 0;
    for (index_t i = 0; i < m; ++i) abs_column_sum += std::abs(a(i, j));
    result = std::max(result, abs_column_sum);
  }
  return result;
}
} // namespace satyr
