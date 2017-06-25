#pragma once

#include <satyr/linear_algebra/concept.h>
#include <satyr/linear_algebra/operation_matrix_expression.h>
#include <satyr/linear_algebra/vector.h>

namespace satyr {
//------------------------------------------------------------------------------
// get_leading_dimension
//------------------------------------------------------------------------------
template <OperationMatrix A>
index_t get_leading_dimension(const A& a) {
  return get_stride<1>(a);
}

template <OperationMatrix A>
  requires detail::match_transposed_matrix<A>
index_t get_leading_dimension(const A& a) {
  return get_stride<0>(a);
}

//------------------------------------------------------------------------------
// get_underlying_uplo
//------------------------------------------------------------------------------
template <TriangularOperationMatrix A>
uplo_t get_underlying_uplo(const A& a) {
  return structure_t<A>::uplo;
}

template <TriangularOperationMatrix A>
  requires detail::match_transposed_matrix<A>
uplo_t get_underlying_uplo(const A& a) {
  return flip_uplo_v<structure_t<A>::uplo>;
}

//------------------------------------------------------------------------------
// get_underlying_shape
//------------------------------------------------------------------------------
template <OperationMatrix A>
shape<2> get_underlying_shape(const A& a) {
  return a.shape();
}

template <OperationMatrix A>
  requires detail::match_transposed_matrix<A>
shape<2> get_underlying_shape(const A& a) {
  return {get_extent<1>(a), get_extent<0>(a)};
}

//------------------------------------------------------------------------------
// get_diagonal
//------------------------------------------------------------------------------
template <Matrix A>
auto get_diagonal(A&& a) {
  auto stride = get_stride<0>(a) + get_stride<1>(a);
  auto n = std::min(get_extent<0>(a), get_extent<1>(a));
  auto subshape = satyr::subshape<1>{{n}, {stride}};
  return make_n_array_subview<general_structure>(a.data(), subshape);
}
} // namespace satyr
