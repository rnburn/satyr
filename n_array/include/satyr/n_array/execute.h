#pragma once

#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/concept.h>
#include <satyr/for.h>
#include <satyr/for_each_index.h>
#include <satyr/execution_policy.h>

namespace satyr {
//------------------------------------------------------------------------------
// execute
//------------------------------------------------------------------------------
template <size_t K, FlatEvaluator Evaluator, Policy Policy>
void execute(const n_array_expression<K, general_structure, Evaluator, Policy>&
                 expression) {
  for_(simd_v | expression.policy(), 0, get_num_elements(expression),
       expression.evaluator());
}

template <size_t K, KEvaluator<K> Evaluator, Policy Policy>
void execute(const n_array_expression<K, general_structure, Evaluator, Policy>&
                 expression) {
  auto shape = get_shape(expression);
  auto evaluator = expression.evaluator();
  for_each_index(simd_v | expression.policy(), shape.extents(),
                 [shape, evaluator](auto... indexes) -> decltype(auto) {
                   return evaluator(shape, indexes...);
                 });
}
} // namespace satyr
