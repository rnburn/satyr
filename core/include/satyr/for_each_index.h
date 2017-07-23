#pragma once

#include <satyr/serial_for.h>
#include <satyr/serial_for_each_index.h>
#include <satyr/parallel_for.h>
#include <satyr/blocked_range.h>
#include <satyr/matrix.h>

namespace satyr {
//------------------------------------------------------------------------------
// for_each_index_triangular
//------------------------------------------------------------------------------
template <uplo_t Uplo, Policy Policy, IndexFunctor<2> Functor>
  requires has_policy_v<grainsize, Policy>
void for_each_index_triangular(Policy policy, index_t n, Functor f) {
  auto grainsize_outer = subdivide(get_policy<grainsize>(policy), n + 1);
  auto n_div_2 = n / 2;
  auto p = n / 2 + (n % 2);
  for_(grainsize_outer, 0, p, [=](index_t j1) {
    auto j2 = n - j1 - 1;
    if (j1 != j2) {
      detail::for_each_index_triangular_impl<Uplo>(policy, j1, n, f);
      detail::for_each_index_triangular_impl<Uplo>(policy, j2, n, f);
    } else {
      detail::for_each_index_triangular_impl<Uplo>(policy, j1, n, f);
    }
  });
}
} // namespace satyr
