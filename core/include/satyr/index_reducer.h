#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>

namespace satyr {
//------------------------------------------------------------------------------
// split_reduction
//------------------------------------------------------------------------------
constexpr struct split_reduction { } split_reduction_v; 

//------------------------------------------------------------------------------
// IndexReducer
//------------------------------------------------------------------------------
template <class R, class T>
concept bool IndexReducer = requires(R r, index_t i, T (*f)(index_t)) {
  typename R::value_type;
  R{r, split_reduction_v};
  r(no_policy_v, i, i, f);
  r.join(r);
};
}  // namespace satyr
