#pragma once

#include <satyr/concept.h>
#include <satyr/n_array/n_array_expression.h>

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


} // namespace satyr
