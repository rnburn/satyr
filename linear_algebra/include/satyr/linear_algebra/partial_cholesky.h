#pragma once

#include <tuple>

#include <satyr/matrix.h>
#include <satyr/linear_algebra/vector.h>
#include <satyr/linear_algebra/matrix.h>
#include <satyr/linear_algebra/concept.h>
#include <satyr/linear_algebra/utility.h>
#include <satyr/linear_algebra/operation_matrix_expression.h>
#include <satyr/blas_lapack.h>

namespace satyr {
//------------------------------------------------------------------------------
// inplace_cholesky_factorize_until_failure
//------------------------------------------------------------------------------
template <SymmetricMatrix A>
  requires is_blas_scalar_v<value_type_t<A>> && is_writable_v<A>
std::tuple<lower_triangular_matrix_subview<value_type_t<A>>,
           symmetric_matrix_subview<value_type_t<A>>>
inplace_cholesky_factorize_until_failure(A&& a) {
  auto n = get_extent<0>(a);
  auto lda = get_leading_dimension(a);
  auto rcode = potrf(uplo_t::lower, n, a.data(), lda);
  if (rcode == 0) {
    return {structure_cast<lower_triangular_structure>(make_view(a)),
            symmetric_matrix_subview<value_type_t<A>>{}};
  } else {
    index_t diagonal_index = rcode - 1;
    auto& a_general = structure_cast<general_structure>(a);
    auto factorization =
        a_general(range{0, diagonal_index}, range{0, diagonal_index});
    auto remainder =
        a_general(range{diagonal_index, n}, range{diagonal_index, n});
    return {structure_cast<lower_triangular_structure>(factorization),
            structure_cast<symmetric_structure>(remainder)};
  }
}
} // namespace satyr
