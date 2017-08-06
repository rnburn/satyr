#pragma once

#include <satyr/blocked_range.h>
#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/index_reducer.h>
#include <satyr/matrix.h>
#include <satyr/serial_for.h>
#include <satyr/traits.h>

namespace satyr {
//------------------------------------------------------------------------------
// reduce
//------------------------------------------------------------------------------
template <Policy Policy, IndexReducer Reducer, IndexFunctor<1> Functor>
  requires !has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, 1>,
                                 value_type_t<Reducer>>
void reduce(Policy policy, index_t first, index_t last, Reducer& reducer,
            Functor f) {
  reducer(policy, first, last, f);
}

//------------------------------------------------------------------------------
// reduce_each_index
//------------------------------------------------------------------------------
// k-cell
template <Policy Policy, size_t K, IndexReducer Reducer, 
          IndexFunctor<K> Functor>
  requires !has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, K>,
                                 value_type_t<Reducer>>
void reduce_each_index(Policy policy, std::array<index_t, K> extents, 
                       Reducer& reducer, Functor f) {
  if constexpr(K == 1) { 
    reducer(policy, 0, extents[0], f); 
  } else {
    for_(serial_v, 0, extents[K - 1], [=, &reducer](index_t i) {
      auto f_prime = [=](auto... indexes) { return f(indexes..., i); };
      reduce_each_index(
          policy, reinterpret_cast<const std::array<index_t, K - 1>&>(extents),
          reducer, f_prime);
    });
  }
}

// k-cell blocked-range
namespace detail {
template <size_t I, Policy Policy, size_t K, class Reducer, class Functor>
void blocked_range_reduce_each_index_impl(Policy policy,
                                       const k_blocked_range<K>& range,
                                       Reducer& reducer, Functor f) {
  if constexpr(I == 0) {
      reducer(policy, range.template first<0>(), range.template last<0>(), f);
  } else {
    for_(serial_v, range.template first<I>(), range.template last<I>(),
         [policy, &range, &reducer, f](index_t i) {
           auto f_prime = [=](auto... indexes) { return f(indexes..., i); };
           blocked_range_reduce_each_index_impl<I - 1>(policy, range, reducer, 
                                                       f_prime);
         });
  }
}
} // namespace detail

template <Policy Policy, size_t K, IndexReducer Reducer, 
          IndexFunctor<K> Functor>
  requires !has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, K>,
                                 value_type_t<Reducer>>
void reduce_each_index(Policy policy, const k_blocked_range<K>& range,
    Reducer& reducer, Functor f) {
  detail::blocked_range_reduce_each_index_impl<K-1>(policy, range, reducer, f);
}

//------------------------------------------------------------------------------
// reduce_each_index_triangular
//------------------------------------------------------------------------------
namespace detail {
template <uplo_t Uplo, class Policy, class Reducer, class Functor>
    requires Uplo == uplo_t::lower 
void reduce_each_index_triangular_impl(Policy policy, index_t j, index_t n,
                                       Reducer& reducer, Functor f) {
  reducer(policy, j, n, [=](index_t i) { return f(i, j); });
}

template <uplo_t Uplo, class Policy, class Reducer, class Functor>
    requires Uplo == uplo_t::upper 
void reduce_each_index_triangular_impl(Policy policy, index_t j, index_t n,
                                       Reducer& reducer, Functor f) {
  reducer(policy, 0, j+1, [=](index_t i) { return f(i, j); });
}
}  // namespace detail

// n
template <uplo_t Uplo, Policy Policy, IndexReducer Reducer,
          IndexFunctor<2> Functor>
  requires !has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, 2>,
                                 value_type_t<Reducer>>
void reduce_each_index_triangular(Policy policy, index_t n, Reducer& reducer,
                                  Functor f) {
  for_(no_policy_v, 0, n, [=](index_t j) {
    detail::reduce_each_index_triangular_impl<Uplo>(policy, j, n, reducer, f);
  });
}

// triangular blocked-range
template <uplo_t Uplo, Policy Policy, IndexReducer Reducer, 
          IndexFunctor<2> Functor>
  requires !has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, 2>,
                                 value_type_t<Reducer>>
void reduce_each_index_triangular(Policy policy,
                                  const triangular_blocked_range<Uplo>& range,
                                  Reducer& reducer, Functor f) {
  if (range.is_column_range()) {
    auto [j_first, j_last] = range.columns();
    auto n = range.n();
    for_(no_policy_v, j_first, j_last, [=, &reducer](index_t j) {
      detail::reduce_each_index_triangular_impl<Uplo>(policy, j, n, reducer, f);
    });
  } else {
    auto [j, i_first, i_last] = range.column_rows();
    reducer(policy, i_first, i_last, [=](index_t i) { return f(i, j); });
  }
}
} // namespace satyr
