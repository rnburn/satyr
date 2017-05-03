#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/expression.h>
#include <satyr/policy.h>

namespace satyr {
//------------------------------------------------------------------------------
// operator<<
//------------------------------------------------------------------------------
template <Expressible Expressible, Policy Policy>
auto operator<<(Expressible&& expressible, Policy policy) {
  return make_expression(expressible);
}
} // namespace satyr
