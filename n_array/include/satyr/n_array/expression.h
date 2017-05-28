#pragma once

#include <satyr/concept.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/scalar_expression.h>
#include <satyr/n_array/n_array_evaluator.h>
#include <satyr/n_array/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// make_expression
//------------------------------------------------------------------------------
template <Scalar Scalar>
scalar_expression<Scalar, no_policy> make_expression(Scalar value) {
  return {value, no_policy_v};
}

template <Scalar Scalar, Policy Policy>
scalar_expression<Scalar, Policy> make_expression(
    const scalar_expression<Scalar, Policy>& expression) {
  return expression;
}

template <size_t K, Structure Structure, Evaluator<K> Evaluator, Policy Policy>
n_array_expression<K, Structure, Evaluator, Policy> make_expression(
    const n_array_expression<K, Structure, Evaluator, Policy>& expression) {
  return expression;
}

template <Scalar T, size_t K>
n_array_expression<K, general_structure, flat_n_array_evaluator<T>,
                   no_policy>
make_expression(n_array<T, K, general_structure>& array) {
  return {array.shape(), flat_n_array_evaluator(array.data()),
          no_policy_v};
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression<K, Structure, n_array_evaluator<T, K>, no_policy>
make_expression(n_array<T, K, Structure>& array) {
  return {array.shape(), n_array_evaluator<T, K>{array.data()}, no_policy_v};
}

template <Scalar T, size_t K>
n_array_expression<K, general_structure, flat_n_array_evaluator<T>,
                   no_policy>
make_expression(const n_array_view<T, K, general_structure>& array) {
  return {array.shape(), flat_n_array_evaluator(array.data()),
          no_policy_v};
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression<K, Structure, n_array_evaluator<T, K>, no_policy>
make_expression(const n_array_view<T, K, Structure>& array) {
  return {array.shape(), n_array_evaluator<T, K>{array.data()}, no_policy_v};
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression<K, Structure, n_array_subview_evaluator<T, K>, no_policy>
make_expression(const n_array_subview<T, K, Structure>& array) {
  return {
      array.shape(),
      n_array_subview_evaluator<T, K>{array.data(), array.shape().strides()},
      no_policy_v};
}
} // namespace satyr
