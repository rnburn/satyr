#pragma once

#include <satyr/linear_algebra/vector.h>
#include <satyr/linear_algebra/matrix.h>
#include <satyr/linear_algebra/concept.h>
#include <satyr/linear_algebra/operation_matrix_expression.h>
#include <satyr/blas_lapack.h>

namespace satyr {
//------------------------------------------------------------------------------
// inplace_product
//------------------------------------------------------------------------------
template <GeneralOperationMatrix A, Vector X, Vector Y>
  requires BlasScalar<value_type_t<A>> &&
           is_same_v<value_type_t<A>, value_type_t<X>, value_type_t<Y>> &&
           is_writable_v<Y>
void inplace_product(value_type_t<A> alpha, const A& a, const X& x,
                     value_type_t<A> beta, Y&& y) {
  
}
} // namespace satyr
