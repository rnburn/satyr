#pragma once

#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/n_array_reduction_expression.h>
#include <satyr/n_array/concept.h>
#include <satyr/for.h>
#include <satyr/reduce.h>
#include <satyr/execution_policy.h>

namespace satyr {
//------------------------------------------------------------------------------
// execute
//------------------------------------------------------------------------------
// n_array_expression, flat
template <size_t K, FlatEvaluator Evaluator, Policy Policy>
void execute(const n_array_expression<K, general_structure, Evaluator, Policy>&
                 expression) {
  for_(simd_v | expression.policy(), 0, get_num_elements(expression),
       expression.evaluator());
}

// n_array_expression, k
template <size_t K, KEvaluator<K> Evaluator, Policy Policy>
void execute(const n_array_expression<K, general_structure, Evaluator, Policy>&
                 expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  for_each_index(simd_v | expression.policy(), shape.extents(),
                 [shape, evaluator](auto... indexes) {
                    return evaluator(shape, indexes...);
                 });
}

// n_array_expression, triangular
template <Structure Structure, KEvaluator<2> Evaluator, Policy Policy>
  requires Structure::uplo == uplo_t::upper ||
           Structure::uplo == uplo_t::lower
void execute(const n_array_expression<2, Structure, Evaluator, Policy>&
                 expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  for_each_index_triangular<Structure::uplo>(
      simd_v | expression.policy(), get_extent<0>(shape),
      [shape, evaluator](index_t i, index_t j) {
        return evaluator(shape, i, j);
      });
}

// n_array_reduction_expression, flat
template <size_t K, IndexReducer Reducer, FlatEvaluator Evaluator,
          Policy Policy>
value_type_t<Reducer> execute(
    const n_array_reduction_expression<K, general_structure, Reducer, Evaluator,
                                       Policy>& expression) {
  Reducer reducer;
  reduce(simd_v | expression.policy(), 0, get_num_elements(expression), reducer,
         expression.evaluator());
  return reducer.value();
}

// n_array_reduction_expression, k
template <size_t K, IndexReducer Reducer, KEvaluator<K> Evaluator,
          Policy Policy>
value_type_t<Reducer> execute(
    const n_array_reduction_expression<K, general_structure, Reducer, Evaluator,
                                       Policy>& expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  Reducer reducer;
  reduce_each_index(simd_v | expression.policy(), shape.extents(), reducer,
                    [shape, evaluator](auto... indexes) {
                      return evaluator(shape, indexes...);
                    });
  return reducer.value();
}

// n_array_reduction_expression, triangular
template <Structure Structure, IndexReducer Reducer, KEvaluator<2> Evaluator,
          Policy Policy>
  requires Structure::uplo == uplo_t::upper ||
           Structure::uplo == uplo_t::lower
value_type_t<Reducer> execute(
    const n_array_reduction_expression<2, Structure, Reducer, Evaluator,
                                       Policy>& expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  Reducer reducer;
  reduce_each_index_triangular<Structure::uplo>(
      simd_v | expression.policy(), shape.extent(0), reducer,
      [shape, evaluator](index_t i, index_t j) {
        return evaluator(shape, i, j);
      });
  return reducer.value();
}

//------------------------------------------------------------------------------
// execute_with_exit
//------------------------------------------------------------------------------
// n_array_expression, flat
template <size_t K, FlatEvaluator Evaluator, Policy Policy>
bool execute_with_exit(
    const n_array_expression<K, general_structure, Evaluator, 
                             Policy>& expression) {
  return for_with_exit(simd_v | expression.policy(), 0,
                       get_num_elements(expression), expression.evaluator());
}

// n_array_expression, k
template <size_t K, KEvaluator<K> Evaluator, Policy Policy>
bool execute_with_exit(const n_array_expression<K, general_structure, Evaluator,
                                                Policy>& expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  return for_each_index_with_exit(simd_v | expression.policy(),
                                    shape.extents(),
                                    [shape, evaluator](auto... indexes) {
                                      return evaluator(shape, indexes...);
                                    });
}

// n_array_expression, triangular
template <Structure Structure, KEvaluator<2> Evaluator, Policy Policy>
  requires Structure::uplo == uplo_t::upper ||
           Structure::uplo == uplo_t::lower
bool execute_with_exit(
    const n_array_expression<2, Structure, Evaluator, Policy>& expression) {
  auto shape = expression.shape();
  auto evaluator = expression.evaluator();
  return for_each_index_triangular_with_exit<Structure::uplo>(
      simd_v | expression.policy(), get_extent<0>(shape),
      [shape, evaluator](index_t i, index_t j) {
        return evaluator(shape, i, j);
      });
}
} // namespace satyr
