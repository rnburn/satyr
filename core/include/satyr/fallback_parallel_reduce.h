// Fallback to serial version if there's no task-based parallelism enabled.
#pragma once

#include <satyr/serial_reduce.h>

namespace satyr {
//------------------------------------------------------------------------------
// reduce
//------------------------------------------------------------------------------
template <Policy Policy, IndexReducer Reducer, IndexFunctor<1> Functor>
  requires has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, 1>,
                                 value_type_t<Reducer>>
void reduce(Policy policy, index_t first, index_t last, Reducer& reducer,
            Functor f) {
  reduce(policy | serial_v, first, last, reducer, f);
}

//------------------------------------------------------------------------------
// reduce_each_index
//------------------------------------------------------------------------------
template <Policy Policy, size_t K, IndexReducer Reducer, 
          IndexFunctor<K> Functor>
  requires has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, K>,
                                 value_type_t<Reducer>>
void reduce_each_index(Policy policy, std::array<index_t, K> extents, 
                       Reducer& reducer, Functor f) {
  reduce(policy | serial_v, extents, reducer, f);
}

//------------------------------------------------------------------------------
// reduce_each_index_triangular
//------------------------------------------------------------------------------
template <uplo_t Uplo, Policy Policy, IndexReducer Reducer,
          IndexFunctor<2> Functor>
  requires has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, 2>,
                                 value_type_t<Reducer>>
void reduce_each_index_triangular(Policy policy, index_t n, Reducer& reducer,
                                  Functor f) {
  reduce_each_index_triangular(policy | serial_v, n, reducer, f);
}
} // namespace satyr
