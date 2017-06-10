#pragma once

#include <optional>

#include <satyr/matrix.h>
#include <satyr/linear_algebra/vector.h>
#include <satyr/linear_algebra/matrix.h>
#include <satyr/linear_algebra/concept.h>
#include <satyr/linear_algebra/utility.h>
#include <satyr/linear_algebra/operation_matrix_expression.h>
#include <satyr/blas_lapack.h>

namespace satyr {
//------------------------------------------------------------------------------
// inplace_cholesky_factorize
//------------------------------------------------------------------------------
template <SymmetricMatrix A>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_writable_v<A>
auto inplace_cholesky_factorize(A&& a) {
  auto n = get_extent<0>(a);
  auto lda = get_leading_dimension(a);
  auto rcode = potrf(uplo_t::lower, n, a.data(), lda);
  using View = uncvref_t<decltype(
      structure_cast<lower_triangular_structure>(make_view(a)))>;
  std::optional<View> result;
  if (rcode == 0)
    result = structure_cast<lower_triangular_structure>(make_view(a));
  return result;
}

//------------------------------------------------------------------------------
// cholesky_factorize
//------------------------------------------------------------------------------
template <SymmetricMatrix A>
  requires is_blas_scalar_v<value_type_t<A>> &&
           is_writable_v<A>
std::optional<lower_triangular_matrix<value_type_t<A>>> cholesky_factorize(
    const A& a) {
  symmetric_matrix<value_type_t<A>> b{a};
  std::optional<lower_triangular_matrix<value_type_t<A>>> result;
  if (inplace_cholesky_factorize(b))
    result = std::move(structure_cast<lower_triangular_structure>(b));
  return result;
}
} // namespace satyr
