#pragma once

#include <satyr/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// Matrix
//------------------------------------------------------------------------------
template <class T>
concept bool Matrix = NArray<T> && num_dimensions_v<T> == 2;

//------------------------------------------------------------------------------
// OperationMatrix
//------------------------------------------------------------------------------
template <KEvaluator<2> Evaluator>
class transpose_evaluator;

namespace detail {
template <class T>
constexpr bool match_transposed_matrix = false;

template <Scalar Scalar, Policy Policy>
constexpr bool match_transposed_matrix<n_array_expression<
    2, general_structure, transpose_evaluator<n_array_evaluator<Scalar, 2>>,
    Policy>> = true;
} // namespace detail
} // namespace satyr
