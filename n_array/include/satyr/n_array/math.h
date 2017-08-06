#pragma once

#include <satyr/functional.h>
#include <satyr/math_reducer.h>
#include <satyr/n_array/execute.h>
#include <satyr/n_array/expression.h>
#include <satyr/n_array/map.h>
#include <satyr/n_array/n_array_reduction_expression.h>
#include <cmath>

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

//------------------------------------------------------------------------------
// max_element
//------------------------------------------------------------------------------
template <RealNArrayExpressible Expressible>
value_type_t<Expressible> max_element(const Expressible& expressible) {
  auto expression = make_expression(expressible);
  auto reduction_expression =
      make_n_array_reduction_expression<structure_t<Expressible>,
                                        max_reducer<value_type_t<Expressible>>>(
          expression.shape(), expression.evaluator(), expression.policy());
  return execute(reduction_expression);
}

//------------------------------------------------------------------------------
// min_element
//------------------------------------------------------------------------------
template <RealNArrayExpressible Expressible>
value_type_t<Expressible> min_element(const Expressible& expressible) {
  auto expression = make_expression(expressible);
  auto reduction_expression =
      make_n_array_reduction_expression<structure_t<Expressible>,
                                        min_reducer<value_type_t<Expressible>>>(
          expression.shape(), expression.evaluator(), expression.policy());
  return execute(reduction_expression);
}

//------------------------------------------------------------------------------
// sum_elements
//------------------------------------------------------------------------------
template <NArrayExpressible Expressible>
value_type_t<Expressible> sum_elements(const Expressible& expressible) {
  auto expression = make_expression(expressible);
  auto reduction_expression =
      make_n_array_reduction_expression<structure_t<Expressible>,
                                        sum_reducer<value_type_t<Expressible>>>(
          expression.shape(), expression.evaluator(), expression.policy());
  return execute(reduction_expression);
}
} // namespace satyr
