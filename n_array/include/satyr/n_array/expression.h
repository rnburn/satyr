#pragma once

#include <satyr/concept.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/n_array_evaluator.h>
#include <satyr/n_array.h>

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
n_array_expression<K, Structure, contiguous_n_array_evaluator<T*>>
make_expression(n_array<T, K, Structure>& array) {
  return n_array_expression<K, Structure, contiguous_n_array_evaluator<T*>>(
      contiguous_n_array_evaluator(array.data()));
}

template <Scalar T, size_t K, Structure Structure>
n_array_expression<K, Structure, contiguous_n_array_evaluator<T*>>
make_expression(const n_array_view<T, K, Structure>& array) {
  return n_array_expression<K, Structure, contiguous_n_array_evaluator<T*>>(
      contiguous_n_array_evaluator(array.data()));
}
} // namespace satyr
