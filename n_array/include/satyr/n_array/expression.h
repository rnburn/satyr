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
Scalar make_expression(Scalar value) { return value; }

template <size_t K, Structure Structure, Evaluator<K> Evaluator>
n_array_expression<K, Structure, Evaluator> 
make_expression(const n_array_expression<K, Structure, Evaluator>& expression) {
  return expression;
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression<K, Structure, contiguous_n_array_evaluator<T>>
make_expression(n_array<T, K, Structure>& array) {
  return {array.shape(), contiguous_n_array_evaluator(array.data())};
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression<K, Structure, contiguous_n_array_evaluator<T>>
make_expression(const n_array_view<T, K, Structure>& array) {
  return {array.shape(), contiguous_n_array_evaluator(array.data())};
}

template <Scalar Scalar>
scalar_expression<Scalar, no_policy> make_expression2(Scalar value) {
  return {value, no_policy_v};
}

template <size_t K, Structure Structure, Evaluator<K> Evaluator, Policy Policy>
n_array_expression2<K, Structure, Evaluator, Policy> make_expression2(
    const n_array_expression2<K, Structure, Evaluator, Policy>& expression) {
  return expression;
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression2<K, Structure, contiguous_n_array_evaluator<T>, no_policy>
make_expression2(n_array<T, K, Structure>& array) {
  return {array.shape(), contiguous_n_array_evaluator(array.data()),
          no_policy_v};
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression2<K, Structure, contiguous_n_array_evaluator<T>, no_policy>
make_expression2(const n_array_view<T, K, Structure>& array) {
  return {array.shape(), contiguous_n_array_evaluator(array.data()),
          no_policy_v};
}
} // namespace satyr
