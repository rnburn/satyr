#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_reduce.h>
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
  tbb_reducer(Policy policy, const Reducer& reducer, F f)
      : policy_{policy}, reducer_{reducer}, f_{f} {}

  tbb_reducer(tbb_reducer& other, tbb::split)
      : policy_{other.policy},
        reducer_{other.reducer_, split_reduction_v},
        f_{other.f} {}

  void operator()(tbb::blocked_range<index_t> range) {
    reducer_(policy_, range.begin(), range.end(), f_);
  }

  void join(tbb_reducer& other) {
    reducer_.join(other.reducer_);
  }

  value_type_t<Reducer> value() const { return reducer_.value(); }

 private:
  Policy policy_;
  Reducer reducer_;
  F f_;
};
} // namespace detail

//------------------------------------------------------------------------------
// reduce
//------------------------------------------------------------------------------
template <Policy Policy, IndexFunctor<1> F,
          IndexReducer<index_functor_codomain_t<F, 1>> Reducer>
  requires has_policy_v<grainsize, Policy>
value_type_t<Reducer> reduce(Policy policy, index_t first, index_t last,
                             Reducer reducer, F f) {
  auto tbb_reducer = detail::tbb_reducer(policy, reducer, f);
  auto grainsize = get_policy<satyr::grainsize>(policy);
  tbb::parallel_reduce(
      tbb::blocked_range<index_t>{first, last,
                                  static_cast<size_t>(grainsize.value)},
      tbb_reducer);
  return tbb_reducer.value();
}
} // namespace satyr
