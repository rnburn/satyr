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
// inplace_product
//------------------------------------------------------------------------------
// gemv
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

// gemm
template <GeneralOperationMatrix A, GeneralOperationMatrix B,
          GeneralOperationMatrix C>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>, value_type_t<C>> &&
           is_writable_v<C>
void inplace_product(value_type_t<A> alpha, const A& a, const B& b,
                     value_type_t<A> beta, C&& c) {
  auto a_n = get_extent<1>(a);
  auto lda = get_leading_dimension(a);

  auto ldb = get_leading_dimension(b);

  auto [c_m, c_n] = c.shape();
  auto ldc = get_leading_dimension(c);

  gemm(matrix_operation_v<A>, matrix_operation_v<B>, c_m, c_n, a_n, alpha,
       a.data(), lda, b.data(), ldb, beta, c.data(), ldc);
}

template <GeneralOperationMatrix A, GeneralOperationMatrix B,
          GeneralOperationMatrix C>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>, value_type_t<C>> &&
           is_writable_v<C>
void inplace_product(const A& a, const B& b, C&& c) {
  inplace_product(1, a, b, 0, c);
}

// symv
template <SymmetricMatrix A, Vector X, Vector Y>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>, value_type_t<Y>> &&
           is_writable_v<Y>
void inplace_product(value_type_t<A> alpha, const A& a, const X& x,
                     value_type_t<A> beta, Y&& y) {
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto incx = get_stride<0>(x);

  auto incy = get_stride<0>(y);

  symv(structure_t<A>::uplo, a_m, alpha, a.data(), lda, x.data(), incx, beta,
       y.data(), incy);
}

template <SymmetricMatrix A, Vector X, Vector Y>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>, value_type_t<Y>> &&
           is_writable_v<Y>
void inplace_product(const A& a, const X& x, Y&& y) {
  inplace_product(1, a, x, 0, y);
}

// symm
template <SymmetricMatrix A, GeneralMatrix B, GeneralMatrix C>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>, value_type_t<C>> &&
           is_writable_v<C>
void inplace_product(value_type_t<A> alpha, const A& a, const B& b,
                     value_type_t<A> beta, C&& c) {
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto b_n = get_extent<1>(b);
  auto ldb = get_leading_dimension(b);

  auto ldc = get_leading_dimension(c);

  symm(matrix_side_t::left, structure_t<A>::uplo, a_m, b_n, alpha, a.data(),
       lda, b.data(), ldb, beta, c.data(), ldc);
}

template <GeneralMatrix A, SymmetricMatrix B, GeneralMatrix C>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>, value_type_t<C>> &&
           is_writable_v<C>
void inplace_product(value_type_t<A> alpha, const A& a, const B& b,
                     value_type_t<A> beta, C&& c) {
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto b_n = get_extent<1>(b);
  auto ldb = get_leading_dimension(b);

  auto ldc = get_leading_dimension(c);

  symm(matrix_side_t::right, structure_t<B>::uplo, a_m, b_n, alpha, b.data(),
       ldb, a.data(), lda, beta, c.data(), ldc);
}

template <class A, class B, GeneralMatrix C>
  requires ((SymmetricMatrix<A> && GeneralMatrix<B>) ||
            (GeneralMatrix<A> && SymmetricMatrix<B>)) &&
           is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>, value_type_t<C>> &&
           is_writable_v<C>
void inplace_product(const A& a, const B& b, C&& c) {
  inplace_product(1, a, b, 0, c);
}

//------------------------------------------------------------------------------
// product
//------------------------------------------------------------------------------
// gemv
template <GeneralOperationMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> product(value_type_t<A> alpha, const A& a, const X& x) {
  vector<value_type_t<A>> y(get_extent<0>(a));
  inplace_product(alpha, a, x, 0, y);
  return y;
}

template <GeneralOperationMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> product(const A& a, const X& x) {
  return product(1, a, x);
}

// gemm
template <GeneralOperationMatrix A, GeneralOperationMatrix B>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>>
matrix<value_type_t<A>> product(value_type_t<A> alpha, const A& a, const B& b) {
  matrix<value_type_t<A>> c(get_extent<0>(a), get_extent<1>(b));
  inplace_product(alpha, a, b, 0, c);
  return c;
}

template <GeneralOperationMatrix A, GeneralOperationMatrix B>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>>
matrix<value_type_t<A>> product(const A& a, const B& b) {
  return product(1, a, b);
}

// symv
template <SymmetricMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> product(value_type_t<A> alpha, const A& a, const X& x) {
  vector<value_type_t<A>> y(get_extent<0>(a));
  inplace_product(alpha, a, x, 0, y);
  return y;
}

template <SymmetricMatrix A, Vector X>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>>
vector<value_type_t<A>> product(const A& a, const X& x) {
  return product(1, a, x);
}

// symm
template <class A, class B>
  requires ((SymmetricMatrix<A> && GeneralMatrix<B>) ||
            (GeneralMatrix<A> && SymmetricMatrix<B>)) &&
           is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>>
matrix<value_type_t<A>> product(value_type_t<A> alpha, const A& a, const B& b) {
  matrix<value_type_t<A>> c(get_extent<0>(a), get_extent<1>(b));
  inplace_product(alpha, a, b, 0, c);
  return c;
}

template <class A, class B>
  requires ((SymmetricMatrix<A> && GeneralMatrix<B>) ||
            (GeneralMatrix<A> && SymmetricMatrix<B>)) &&
           is_blas_scalar_v<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<B>>
matrix<value_type_t<A>> product(const A& a, const B& b) {
  return product(1, a, b);
}
} // namespace satyr
