#pragma once

#include <satyr/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// Vector
//------------------------------------------------------------------------------
template <class T>
concept bool Vector = NArray<T>&& num_dimensions_v<T> == 1 &&
                      std::is_same_v<structure_t<T>, general_structure>;

//------------------------------------------------------------------------------
// Matrix
//------------------------------------------------------------------------------
template <class T>
concept bool Matrix = NArray<T> && num_dimensions_v<T> == 2;

//------------------------------------------------------------------------------
// GeneralMatrix
//------------------------------------------------------------------------------
template <class T>
concept bool GeneralMatrix =
    Matrix<T>&& std::is_same_v<structure_t<T>, general_structure>;

//------------------------------------------------------------------------------
// SymmetricMatrix
//------------------------------------------------------------------------------
template <class T>
concept bool SymmetricMatrix =
  Matrix<T> && std::is_same_v<structure_t<T>, symmetric_structure>;

//------------------------------------------------------------------------------
// OperationMatrix
//------------------------------------------------------------------------------
template <KEvaluator<2> Evaluator>
class transpose_evaluator;

namespace detail {
template <class T>
constexpr bool match_transposed_matrix = false;

template <Scalar Scalar, Structure Structure, Policy Policy>
constexpr bool match_transposed_matrix<n_array_expression<
    2, Structure, transpose_evaluator<n_array_evaluator<Scalar, 2>>,
    Policy>> = true;

template <Scalar Scalar, Structure Structure, Policy Policy>
constexpr bool match_transposed_matrix<n_array_expression<
    2, Structure,
    transpose_evaluator<n_array_subview_evaluator<Scalar, 2>>, Policy>> = true;
} // namespace detail

template <class T>
concept bool OperationMatrix =
    Matrix<T> || detail::match_transposed_matrix<uncvref_t<T>>;

//------------------------------------------------------------------------------
// GeneralOperationMatrix
//------------------------------------------------------------------------------
template <class T>
concept bool GeneralOperationMatrix =
    OperationMatrix<T>&& std::is_same_v<structure_t<T>, general_structure>;
}  // namespace satyr
