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
auto inplace_left_solve(const A& a, X&& x) {
  auto n = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto incx = get_stride<0>(x);

  trsv(get_underlying_uplo(a), matrix_operation_v<A>,
       matrix_diagonal_fill_t::general, n, a.data(), lda, x.data(), incx);

  return make_view(x);
}

template <TriangularOperationMatrix A, GeneralMatrix B>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>> &&
           is_writable_v<B>
auto inplace_left_solve(const A& a, value_type_t<A> alpha, B&& b) {
  auto a_n = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto b_n = get_extent<1>(b);
  auto ldb = get_leading_dimension(b);

  trsm(matrix_side_t::left, get_underlying_uplo(a), matrix_operation_v<A>,
       matrix_diagonal_fill_t::general, a_n, b_n, alpha, a.data(), lda,
       b.data(), ldb);

  return make_view(b);
}

template <TriangularOperationMatrix A, GeneralMatrix B>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>> &&
           is_writable_v<B>
auto inplace_left_solve(const A& a, B&& b) {
  return inplace_left_solve(a, 1, b);
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

template <TriangularOperationMatrix A, GeneralMatrix B>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>>
matrix<value_type_t<B>> left_solve(const A& a, value_type_t<A> alpha,
                                   const B& b) {
  matrix<value_type_t<B>> b_copy{b};
  inplace_left_solve(a, alpha, b_copy);
  return b;
}

template <TriangularOperationMatrix A, GeneralMatrix B>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>>
matrix<value_type_t<B>> left_solve(const A& a, const B& b) {
  return left_solve(a, 1, b);
}
} // namespace satyr
