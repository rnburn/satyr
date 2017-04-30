#pragma once

#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/concept.h>
#include <satyr/for.h>
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
    const n_array_expression<K, general_structure, Evaluator>& expression) {}
} // namespace satyr
