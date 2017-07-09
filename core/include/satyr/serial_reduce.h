#pragma once

#include <satyr/execution_policy.h>
#include <satyr/concept.h>
#include <satyr/index_reducer.h>
#include <satyr/traits.h>

namespace satyr {
//------------------------------------------------------------------------------
// reduce
//------------------------------------------------------------------------------
template <Policy Policy, IndexFunctor<1> F,
          IndexReducer<index_functor_codomain_t<F, 1>> Reducer>
  requires !has_policy_v<grainsize, Policy>
value_type_t<Reducer> reduce(Policy policy, index_t first, index_t last,
                             Reducer reducer, F f) {
  reducer(policy, first, last, f);
  return reducer.value();
}
} // namespace satyr
