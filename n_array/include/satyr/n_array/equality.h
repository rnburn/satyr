#pragma once

#include <satyr/n_array/concept.h>

namespace satyr {
template <NArrayExpressible Lhs, NArrayExpressible Rhs>
  requires num_dimensions_v<Lhs> == num_dimensions_v<Rhs>
bool operator==(const Lhs& lhs, const Rhs& rhs) {
  if (lhs.shape() != rhs.shape()) return false;
  auto expression = map([](auto x, auto y) { return x == y; }, lhs, rhs);
  return execute_with_exit(expression);
}

template <NArrayExpressible Lhs, NArrayExpressible Rhs>
  requires num_dimensions_v<Lhs> == num_dimensions_v<Rhs>
bool operator!=(const Lhs& lhs, const Rhs& rhs) {
  return !(lhs == rhs);
}
} // namespace satyr
