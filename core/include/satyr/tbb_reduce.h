#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_reduce_each_index.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

namespace satyr {
//------------------------------------------------------------------------------
// tbb_reducer
//------------------------------------------------------------------------------
namespace detail {
template <class Policy, class Reducer, class F>
class tbb_reducer {
 public:
  tbb_reducer(Policy policy, Reducer& reducer, F f)
      : policy_{policy}, reducer_{reducer}, f_{f} {}

  tbb_reducer(tbb_reducer& other, tbb::split)
      : policy_{other.policy},
        reducer_{other.reducer_, split_reduction_v},
        f_{other.f} {}

  void operator()(tbb::blocked_range<index_t> range) {
    reduce_each_index(policy_ | serial_v, range, reducer_, f_);
  }

  void join(tbb_reducer& other) {
    reducer_.join(other.reducer_);
  }

  value_type_t<Reducer> value() const { return reducer_.value(); }

 private:
  Policy policy_;
  Reducer& reducer_;
  F f_;
};
} // namespace detail

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
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = k_blocked_range(extents, grainsize.value);
  tbb::parallel_reduce(range, tbb_reducer(policy, reducer, f));
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
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = triangular_blocked_range<Uplo>{n, grainsize.value};
  tbb::parallel_reduce(range, tbb_reducer(policy, reducer, f));
}
} // namespace satyr
