#pragma once

#include <satyr/matrix.h>
#include <satyr/linear_algebra/vector.h>
#include <satyr/linear_algebra/matrix.h>
#include <satyr/linear_algebra/concept.h>
#include <satyr/linear_algebra/utility.h>
#include <satyr/linear_algebra/operation_matrix_expression.h>
#include <satyr/blas_lapack.h>

namespace satyr {
//------------------------------------------------------------------------------
// inplace_left_solve
//------------------------------------------------------------------------------
template <TriangularOperationMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>> &&
           is_writable_v<X>
void inplace_left_solve(const A& a, X&& x) {
  auto n = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto incx = get_stride<0>(x);

  trsv(structure_t<A>::uplo, matrix_operation_v<A>,
       matrix_diagonal_fill_t::general, n, a.data(), lda, x.data(), incx);
}

//------------------------------------------------------------------------------
// left_solve
//------------------------------------------------------------------------------
template <TriangularOperationMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> left_solve(const A& a, const X& x) {
  vector<value_type_t<A>> x_copy{x};
  inplace_left_solve(a, x_copy);
  return x_copy;
}
} // namespace satyr
