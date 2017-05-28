#pragma once

#include <mkl.h>
#include <satyr/index.h>
#include <satyr/matrix_operation.h>

namespace satyr {
//------------------------------------------------------------------------------
// get_operation
//------------------------------------------------------------------------------
namespace detail {
inline CBLAS_TRANSPOSE get_operation(matrix_operation operation) {
  switch (operation) {
    case matrix_operation::none:
      return CblasNoTrans;
    case matrix_operation::transpose:
      return CblasTrans;
    case matrix_operation::conjugate_transpose:
      return CblasConjTrans;
  }
}
} // namespace detail

//------------------------------------------------------------------------------
// gemv
//------------------------------------------------------------------------------
#define MAKE_GEMV(SCALAR, PREFIX)                                            \
  inline void gemv(matrix_operation operation_a, index_t m, index_t n,       \
                   SCALAR alpha, const SCALAR* a, index_t lda,               \
                   const SCALAR* x, index_t incx, SCALAR beta, SCALAR* y,    \
                   index_t incy) {                                           \
    cblas_##PREFIX##gemv(CblasColMajor, detail::get_operation(operation_a),  \
                         static_cast<int>(m), static_cast<int>(n), alpha, a, \
                         static_cast<int>(lda), x, static_cast<int>(incx),   \
                         beta, y, static_cast<int>(incy));                   \
  }
MAKE_GEMV(float, s)
MAKE_GEMV(double, d)

#undef MAKE_GEMV
} // namespace satyr
