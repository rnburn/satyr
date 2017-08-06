#pragma once

#include <satyr/serial_for.h>
#include <satyr/blocked_range.h>
#include <satyr/index_reducer.h>
#include <satyr/matrix.h>

namespace satyr {
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
      auto f_prime = [=](auto... indexes) { f(indexes..., i); };
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
           auto f_prime = [=](auto... indexes) { f(indexes..., i); };
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

} // namespace satyr
