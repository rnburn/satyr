#pragma once

#include <satyr/execution_policy.h>
#include <satyr/concept.h>
#include <satyr/index_reducer.h>
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
} // namespace satyr
