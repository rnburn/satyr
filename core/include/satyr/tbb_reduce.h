#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_reduce.h>
#include <satyr/tbb_for.h>
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

  template <class Range>
  void operator()(const Range& range) {
    reduce_each_index(policy_ | serial_v, range.range, reducer_, f_);
  }

  void operator()(const tbb::blocked_range<index_t>& range) {
    reduce(policy_ | serial_v, range.begin(), range.end(), reducer_, f_);
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
// reduce
//------------------------------------------------------------------------------
template <Policy Policy, IndexReducer Reducer, IndexFunctor<1> Functor>
  requires has_policy_v<grainsize, Policy> &&
           std::is_convertible_v<index_functor_codomain_t<Functor, 1>,
                                 value_type_t<Reducer>>
void reduce(Policy policy, index_t first, index_t last, Reducer& reducer,
            Functor f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = tbb::blocked_range<index_t>{first, last,
                                  static_cast<size_t>(grainsize.value)};
  tbb::parallel_reduce(range, tbb_reducer(policy, reducer, f));
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
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = k_blocked_range(extents, grainsize.value);
  tbb::parallel_reduce(detail::tbb_blocked_range(range),
                       tbb_reducer(policy, reducer, f));
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
  tbb::parallel_reduce(detail::tbb_blocked_range(range),
                       tbb_reducer(policy, reducer, f));
}
} // namespace satyr
