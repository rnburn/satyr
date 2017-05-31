#pragma once

#include <satyr/linear_algebra/vector.h>
#include <satyr/linear_algebra/matrix.h>
#include <satyr/linear_algebra/concept.h>
#include <satyr/linear_algebra/utility.h>
#include <satyr/linear_algebra/operation_matrix_expression.h>
#include <satyr/blas_lapack.h>

namespace satyr {
//------------------------------------------------------------------------------
// inplace_product
//------------------------------------------------------------------------------
template <GeneralOperationMatrix A, Vector X, Vector Y>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>, value_type_t<Y>> && 
           is_writable_v<Y>
void inplace_product(value_type_t<A> alpha, const A& a, const X& x,
                     value_type_t<A> beta, Y&& y) {
  auto [a_m, a_n] = get_underlying_shape(a);  
  auto lda = get_leading_dimension(a);

  auto stride_x = get_stride<0>(x);
  auto stride_y = get_stride<0>(y);

  gemv(matrix_operation_v<A>, a_m, a_n, alpha, a.data(), lda, x.data(),
       stride_x, beta, y.data(), stride_y);
}

template <GeneralOperationMatrix A, Vector X, Vector Y>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>, value_type_t<Y>> && 
           is_writable_v<Y>
void inplace_product(const A& a, const X& x, Y&& y) {
  inplace_product(1, a, x, 0, y);
}

//------------------------------------------------------------------------------
// product
//------------------------------------------------------------------------------
template <GeneralOperationMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> product(value_type_t<A> alpha, const A& a, const X& x) {
  vector<value_type_t<A>> y(x.shape());
  inplace_product(alpha, a, x, 0, y);
  return y;
}

template <GeneralOperationMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> product(const A& a, const X& x) {
  return product(1, a, x);
}
} // namespace satyr
