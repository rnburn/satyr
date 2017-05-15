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

template <Scalar Scalar, Policy Policy1, Policy Policy2>
auto attach_policy(const scalar_expression<Scalar, Policy1> expression,
                Policy2 policy2) {
  return scalar_expression(expression.value(), expression.policy() | policy2);
}

template <size_t K, Structure Structure, Evaluator<K> Evaluator, Policy Policy1,
          Policy Policy2>
auto operator<<(
    const n_array_expression<K, Structure, Evaluator, Policy1>& expression,
    Policy2 policy2) {
  return make_n_array_expression<Structure>(expression.shape(),
                                             expression.evaluator(),
                                             expression.policy() | policy2);
}

template <size_t K, Structure Structure, Evaluator<K> Evaluator, Policy Policy1,
          Policy Policy2>
auto attach_policy(
    const n_array_expression<K, Structure, Evaluator, Policy1>& expression,
    Policy2 policy2) {
  return make_n_array_expression<Structure>(expression.shape(),
                                             expression.evaluator(),
                                             expression.policy() | policy2);
}

template <Expressible Expressible, Policy Policy>
auto operator<<(Expressible&& expressible, Policy policy) {
  return attach_policy(make_expression(expressible), policy);
  /* return make_expression(expressible) << policy; */
}
} // namespace satyr
