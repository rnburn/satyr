#pragma once

#include <mkl.h>
#include <satyr/index.h>
#include <satyr/matrix.h>
#include <stdexcept>

namespace satyr {
//------------------------------------------------------------------------------
// get_operation
//------------------------------------------------------------------------------
namespace detail {
inline CBLAS_TRANSPOSE get_operation(matrix_operation_t operation) {
  switch (operation) {
    case matrix_operation_t::none:
      return CblasNoTrans;
    case matrix_operation_t::transpose:
      return CblasTrans;
    case matrix_operation_t::conjugate_transpose:
      return CblasConjTrans;
  }
}
}  // namespace detail

//------------------------------------------------------------------------------
// get_uplo
//------------------------------------------------------------------------------
namespace detail {
inline CBLAS_UPLO get_uplo(uplo_t uplo) {
  switch (uplo) {
    case uplo_t::upper:
      return CblasUpper;
    case uplo_t::lower:
      return CblasLower;
  }
}

inline char get_uplo_char(uplo_t uplo) {
  switch (uplo) {
    case uplo_t::upper:
      return 'U';
    case uplo_t::lower:
      return 'L';
  }
}
}  // namespace detail

//------------------------------------------------------------------------------
// get_side
//------------------------------------------------------------------------------
namespace detail {
inline CBLAS_SIDE get_side(matrix_side_t side) {
  switch (side) {
    case matrix_side_t::left:
      return CblasLeft;
    case matrix_side_t::right:
      return CblasRight;
  }
}
} // namespace detail

//------------------------------------------------------------------------------
// get_unity_diagonal_fill
//------------------------------------------------------------------------------
namespace detail {
inline CBLAS_DIAG get_unity_diagonal_fill(matrix_diagonal_fill_t diagonal_fill) {
  switch (diagonal_fill) {
    case matrix_diagonal_fill_t::unity:
      return CblasUnit;
    case matrix_diagonal_fill_t::general:
      return CblasNonUnit;
    case matrix_diagonal_fill_t::null:
      std::terminate();
  }
}
} // namespace detail

//------------------------------------------------------------------------------
// gemv
//------------------------------------------------------------------------------
#define MAKE_GEMV(SCALAR, PREFIX)                                            \
  inline void gemv(matrix_operation_t operation_a, index_t m, index_t n,     \
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

//------------------------------------------------------------------------------
// gemm
//------------------------------------------------------------------------------
#define MAKE_GEMM(SCALAR, PREFIX)                                              \
  inline void gemm(matrix_operation_t operation_a,                             \
                   matrix_operation_t operation_b, index_t m, index_t n,       \
                   index_t k, SCALAR alpha, const SCALAR* a, index_t lda,      \
                   const SCALAR* b, index_t ldb, SCALAR beta, SCALAR* c,       \
                   index_t ldc) {                                              \
    cblas_##PREFIX##gemm(CblasColMajor, detail::get_operation(operation_a),    \
                         detail::get_operation(operation_b),                   \
                         static_cast<int>(m), static_cast<int>(n),             \
                         static_cast<int>(k), alpha, a, static_cast<int>(lda), \
                         b, static_cast<int>(ldb), beta, c,                    \
                         static_cast<int>(ldc));                               \
  }
MAKE_GEMM(float, s)
MAKE_GEMM(double, d)
#undef MAKE_GEMM

//------------------------------------------------------------------------------
// symv
//------------------------------------------------------------------------------
#define MAKE_SYMV(SCALAR, PREFIX)                                              \
  inline void symv(uplo_t uplo_a, index_t n, SCALAR alpha, const SCALAR* a,    \
                   index_t lda, const SCALAR* x, index_t incx, SCALAR beta,    \
                   SCALAR* y, index_t incy) {                                  \
    cblas_##PREFIX##symv(CblasColMajor, detail::get_uplo(uplo_a),              \
                         static_cast<int>(n), alpha, a, static_cast<int>(lda), \
                         x, static_cast<int>(incx), beta, y,                   \
                         static_cast<int>(incy));                              \
  }
MAKE_SYMV(float, s)
MAKE_SYMV(double, d)
#undef MAKE_SYMV

//------------------------------------------------------------------------------
// symm
//------------------------------------------------------------------------------
#define MAKE_SYMM(SCALAR, PREFIX)                                              \
  inline void symm(matrix_side_t side_a, uplo_t uplo_a, index_t m, index_t n,  \
                   SCALAR alpha, const SCALAR* a, index_t lda,                 \
                   const SCALAR* b, index_t ldb, SCALAR beta, SCALAR* c,       \
                   index_t ldc) {                                              \
    cblas_##PREFIX##symm(CblasColMajor, detail::get_side(side_a),              \
                         detail::get_uplo(uplo_a), static_cast<int>(m),        \
                         static_cast<int>(n), alpha, a, static_cast<int>(lda), \
                         b, static_cast<int>(ldb), beta, c,                    \
                         static_cast<int>(ldc));                               \
  }
MAKE_SYMM(float, s)
MAKE_SYMM(double, d)
#undef MAKE_SYMM

//------------------------------------------------------------------------------
// trsv
//------------------------------------------------------------------------------
#define MAKE_TRSV(SCALAR, PREFIX)                                          \
  void trsv(uplo_t uplo_a, matrix_operation_t operation_a,                 \
            matrix_diagonal_fill_t diagonal_fill_a, index_t n,             \
            const SCALAR* a, index_t lda, SCALAR* x, index_t incx) {       \
    cblas_##PREFIX##trsv(CblasColMajor, detail::get_uplo(uplo_a),          \
                         detail::get_operation(operation_a),               \
                         detail::get_unity_diagonal_fill(diagonal_fill_a), \
                         static_cast<int>(n), a, static_cast<int>(lda), x, \
                         static_cast<int>(incx));                          \
  }
MAKE_TRSV(float, s)
MAKE_TRSV(double, d)
#undef MAKE_TRSV

//------------------------------------------------------------------------------
// trmv
//------------------------------------------------------------------------------
#define MAKE_TRMV(SCALAR, PREFIX)                                          \
  void trmv(uplo_t uplo_a, matrix_operation_t operation_a,                 \
            matrix_diagonal_fill_t diagonal_fill_a, index_t n,             \
            const SCALAR* a, index_t lda, SCALAR* x, index_t incx) {       \
    cblas_##PREFIX##trmv(CblasColMajor, detail::get_uplo(uplo_a),          \
                         detail::get_operation(operation_a),               \
                         detail::get_unity_diagonal_fill(diagonal_fill_a), \
                         static_cast<int>(n), a, static_cast<int>(lda), x, \
                         static_cast<int>(incx));                          \
  }
MAKE_TRMV(float, s)
MAKE_TRMV(double, d)
#undef MAKE_TRMV

//------------------------------------------------------------------------------
// trmm
//------------------------------------------------------------------------------
#define MAKE_TRMM(SCALAR, PREFIX)                                            \
  void trmm(                                                                 \
      matrix_side_t side_a, uplo_t uplo_a, matrix_operation_t operation_a,   \
      matrix_diagonal_fill_t diagonal_fill_a, index_t m, index_t n,          \
      SCALAR alpha, const SCALAR* a, index_t lda, SCALAR* b, index_t ldb) {  \
    cblas_##PREFIX##trmm(CblasColMajor, detail::get_side(side_a),            \
                         detail::get_uplo(uplo_a),                           \
                         detail::get_operation(operation_a),                 \
                         detail::get_unity_diagonal_fill(diagonal_fill_a),   \
                         static_cast<int>(m), static_cast<int>(n), alpha, a, \
                         static_cast<int>(lda), b, static_cast<int>(ldb));   \
  }
MAKE_TRMM(float, s)
MAKE_TRMM(double, d)
#undef MAKE_TRMM

//------------------------------------------------------------------------------
// potrf
//------------------------------------------------------------------------------
#define MAKE_POTRF(SCALAR, SUFFIX)                                          \
  int potrf(uplo_t uplo, index_t n, SCALAR* a, index_t lda) {               \
    auto status = LAPACKE_##SUFFIX##potrf(                                  \
        CblasColMajor, detail::get_uplo_char(uplo), static_cast<int>(n), a, \
        static_cast<int>(lda));                                             \
    return static_cast<int>(status);                                        \
  }
MAKE_POTRF(float, s)
MAKE_POTRF(double, d)
#undef MAKE_POTRF

//------------------------------------------------------------------------------
// potri
//------------------------------------------------------------------------------
#define MAKE_POTRI(SCALAR, SUFFIX)                                          \
  void potri(uplo_t uplo, index_t n, SCALAR* a, index_t lda) {              \
    LAPACKE_##SUFFIX##potri(CblasColMajor, detail::get_uplo_char(uplo),     \
                            static_cast<int>(n), a, static_cast<int>(lda)); \
  }
MAKE_POTRI(float, s)
MAKE_POTRI(double, d)
#undef MAKE_POTRI
}  // namespace satyr
