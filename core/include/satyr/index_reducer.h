#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <type_traits>

namespace satyr {
//------------------------------------------------------------------------------
// split_reduction
//------------------------------------------------------------------------------
constexpr struct split_reduction { } split_reduction_v; 

//------------------------------------------------------------------------------
// IndexReducer
//------------------------------------------------------------------------------
template <class R, class T>
concept bool IndexReducer = requires(R r, const R& cr, index_t i,
                                     T (*f)(index_t)) {
  typename R::value_type;
  R{r, split_reduction_v};
  r(no_policy_v, i, i, f);
  r.join(r);
  requires std::is_same_v<decltype(cr.value()), typename R::value_type>;
};
}  // namespace satyr
