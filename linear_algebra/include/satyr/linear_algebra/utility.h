#pragma once

#include <satyr/linear_algebra/operation_matrix_expression.h>

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
} // namespace satyr
