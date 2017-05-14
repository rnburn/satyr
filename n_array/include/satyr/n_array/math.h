#pragma once

#include <cmath>
#include <satyr/functional.h>
#include <satyr/n_array/map.h>

namespace satyr {
#define MAKE_MATH_FUNCTION(NAME) \
template <NArrayExpressible Expressible> \
  requires detail::is_mappable_v<NAME##_functor, Expressible> \
auto NAME(const Expressible& expressible) { \
  return map(NAME##_functor{}, expressible); \
}
MAKE_MATH_FUNCTION(sqrt)
MAKE_MATH_FUNCTION(log)
MAKE_MATH_FUNCTION(exp)
MAKE_MATH_FUNCTION(sin)
MAKE_MATH_FUNCTION(cos)
MAKE_MATH_FUNCTION(tan)
MAKE_MATH_FUNCTION(abs)
MAKE_MATH_FUNCTION(square)
#undef MAKE_MATH_FUNCTION
} // namespace satyr
