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
template <size_t K, FlatEvaluator Evaluator>
void execute(
    const n_array_expression<K, general_structure, Evaluator>& expression) {
  for_(simd_v, 0, get_num_elements(expression), expression.evaluator());
}

template <size_t K, KEvaluator<K> Evaluator>
void execute(
    const n_array_expression<K, general_structure, Evaluator>& expression) {
  auto shape = get_shape(expression);
  auto evaluator = expression.evaluator();
  for_each_index(simd_v, shape.extents(),
                 [shape, evaluator](auto... indexes) -> decltype(auto) {
                   return evaluator(shape, indexes...);
                 });
}
} // namespace satyr
