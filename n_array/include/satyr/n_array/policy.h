#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/expression.h>
#include <satyr/policy.h>

namespace satyr {
//------------------------------------------------------------------------------
// operator<<
//------------------------------------------------------------------------------
template <Scalar Scalar, Policy Policy1, Policy Policy2>
auto operator<<(const scalar_expression<Scalar, Policy1> expression,
                Policy2 policy2) {
  return scalar_expression(expression.value(), expression.policy() | policy2);
}

template <size_t K, Structure Structure, Evaluator<K> Evaluator, Policy Policy1,
          Policy Policy2>
auto operator<<(
    const n_array_expression2<K, Structure, Evaluator, Policy1>& expression,
    Policy2 policy2) {
  return make_n_array_expression2<Structure>(expression.shape(),
                                             expression.evaluator(),
                                             expression.policy() | policy2);
}

template <Expressible Expressible, Policy Policy>
auto operator<<(Expressible&& expressible, Policy policy) {
  return make_expression2(expressible) << policy;
}
} // namespace satyr
