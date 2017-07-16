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
namespace detail {
template <uplo_t Uplo, class Policy, class Functor>
    requires Uplo == uplo_t::lower 
void for_each_index_triangular_impl(Policy policy, index_t j, index_t n,
                                    Functor f) {
  for_(policy, j, n, [=](index_t i) { f(i, j); });
}

template <uplo_t Uplo, class Policy, class Functor>
    requires Uplo == uplo_t::upper 
void for_each_index_triangular_impl(Policy policy, index_t j, index_t n,
                                    Functor f) {
  for_(policy, 0, j+1, [=](index_t i) { f(i, j); });
}
}  // namespace detail

template <uplo_t Uplo, Policy Policy, IndexFunctor<2> Functor>
  requires !has_policy_v<grainsize, Policy>
void for_each_index_triangular(Policy policy, index_t n, Functor f) {
  for_(no_policy_v, 0, n, [=](index_t j) {
    detail::for_each_index_triangular_impl<Uplo>(policy, j, n, f);
  });
}

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

//------------------------------------------------------------------------------
// for_each_index_triangular_with_exit
//------------------------------------------------------------------------------
namespace detail {
template <uplo_t Uplo, class Policy, class Functor>
    requires Uplo == uplo_t::lower 
bool for_each_index_triangular_with_exit_impl(Policy policy, index_t j,
                                                index_t n, Functor f) {
  return for_with_exit(policy, j, n, [=](index_t i) { return f(i, j); });
}

template <uplo_t Uplo, class Policy, class Functor>
    requires Uplo == uplo_t::upper 
bool for_each_index_triangular_with_exit_impl(Policy policy, index_t j,
                                                index_t n, Functor f) {
  return for_with_exit(policy, 0, j+1, [=](index_t i) { return f(i, j); });
}
}  // namespace detail

template <uplo_t Uplo, Policy Policy, IndexFunctor<2> Functor>
  requires !has_policy_v<grainsize, Policy>
bool for_each_index_triangular_with_exit(Policy policy, index_t n,
                                           Functor f) {
  return for_with_exit(no_policy_v, 0, n, [=](index_t j) {
    return detail::for_each_index_triangular_with_exit_impl<Uplo>(policy, j,
                                                                    n, f);
  });
}
} // namespace satyr
