#pragma once

#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// split_reduction
//------------------------------------------------------------------------------
constexpr struct split_reduction { } split_reduction_v; 

//------------------------------------------------------------------------------
// IndexReducer
//------------------------------------------------------------------------------
template <class R>
concept bool IndexReducer = requires(R r, index_t i) {
  R{r, split_reduction_v};
  r(i, i);
  r.join(r);
};
}  // namespace satyr
