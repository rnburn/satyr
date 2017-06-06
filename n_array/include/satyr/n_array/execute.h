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
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  for_each_index(simd_v | expression.policy(), shape.extents(),
                 [shape, evaluator](auto... indexes) {
                    evaluator(shape, indexes...);
                 });
}

template <Structure Structure, KEvaluator<2> Evaluator, Policy Policy>
  requires Structure::uplo == uplo_t::upper ||
           Structure::uplo == uplo_t::lower
void execute(const n_array_expression<2, Structure, Evaluator, Policy>&
                 expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  for_each_index_triangular<Structure::uplo>(
      simd_v | expression.policy(), get_extent<0>(shape),
      [shape, evaluator](index_t i, index_t j) { evaluator(shape, i, j); });
}

//------------------------------------------------------------------------------
// execute_with_cancel
//------------------------------------------------------------------------------
template <size_t K, FlatEvaluator Evaluator, Policy Policy>
bool execute_with_cancel(
    const n_array_expression<K, general_structure, Evaluator, 
                             Policy>& expression) {
  return for_with_cancel(simd_v | expression.policy(), 0,
                       get_num_elements(expression), expression.evaluator());
}

template <size_t K, KEvaluator<K> Evaluator, Policy Policy>
bool execute_with_cancel(const n_array_expression<K, general_structure, Evaluator,
                                                Policy>& expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  return for_each_index_with_cancel(simd_v | expression.policy(),
                                    shape.extents(),
                                    [shape, evaluator](auto... indexes) {
                                      return evaluator(shape, indexes...);
                                    });
}

template <Structure Structure, KEvaluator<2> Evaluator, Policy Policy>
  requires Structure::uplo == uplo_t::upper ||
           Structure::uplo == uplo_t::lower
void execute_with_cancel(
    const n_array_expression<2, Structure, Evaluator, Policy>& expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  for_each_index_triangular<Structure::uplo>(
      simd_v | expression.policy(), get_extent<0>(shape),
      [shape, evaluator](index_t i, index_t j) {
        return evaluator(shape, i, j);
      });
}
} // namespace satyr
