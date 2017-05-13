#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/scalar_evaluator.h>
#include <satyr/n_array/scalar_expression.h>

namespace satyr {
//------------------------------------------------------------------------------
// convert_evaluator
//------------------------------------------------------------------------------
template <size_t K, Structure FromStructure, Structure ToStructure,
          Evaluator<K> Evaluator>
        requires std::is_same_v<FromStructure, ToStructure>
auto convert_evaluator(const Evaluator& evaluator) {
  return evaluator;
}

//------------------------------------------------------------------------------
// convert_expression
//------------------------------------------------------------------------------
template <Structure ToStructure, size_t K, Structure FromStructure,
          Evaluator<K> Evaluator, Policy Policy>
auto convert_expression(
    const n_array_expression<K, FromStructure, Evaluator, Policy>& expression)
    -> decltype(convert_evaluator<K, FromStructure, ToStructure>(
        expression.evaluator())) {
  return convert_evaluator<K, FromStructure, ToStructure>(
      expression.evaluator());
}

template <Structure ToStructure, Scalar Scalar, Policy Policy>
scalar_evaluator<Scalar> convert_expression(
    const scalar_expression<Scalar, Policy>& expression) {
  return scalar_evaluator(expression.value());
}
} // namespace satyr
