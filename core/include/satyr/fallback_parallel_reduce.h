// Fallback to serial version if there's no task-based parallelism enabled.
#pragma once

#include <satyr/serial_reduce.h>

namespace satyr {
//------------------------------------------------------------------------------
// reduce
//------------------------------------------------------------------------------
template <Policy Policy, IndexFunctor<1> F,
          IndexReducer<index_functor_codomain_t<F, 1>> Reducer>
  requires has_policy_v<grainsize, Policy>
value_type_t<Reducer> reduce(Policy policy, index_t first, index_t last,
                             Reducer reducer, F f) {
  return reduce(policy | serial_v, first, last, f);
}
} // namespace satyr
