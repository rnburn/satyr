#pragma once

#include <satyr/config.h>
#include <satyr/preprocessor.h>
#include <exception>
#include <iostream>
#include <satyr/traits.h>

namespace satyr {
#define MAKE_UNIMPLEMENTED_STUB(RETURN, NAME)                   \
  RETURN NAME(...) {                                            \
    std::cerr << SATYR_STRINGIFY(NAME) " is not implemented\n"; \
    std::terminate();                                           \
  }
// blas
MAKE_UNIMPLEMENTED_STUB(void, dot)
MAKE_UNIMPLEMENTED_STUB(void, gemv)
MAKE_UNIMPLEMENTED_STUB(void, gemm)
MAKE_UNIMPLEMENTED_STUB(void, trsv)
MAKE_UNIMPLEMENTED_STUB(void, trsm)
MAKE_UNIMPLEMENTED_STUB(void, trmv)
MAKE_UNIMPLEMENTED_STUB(void, trmm)
MAKE_UNIMPLEMENTED_STUB(void, symv)
MAKE_UNIMPLEMENTED_STUB(void, symm)
MAKE_UNIMPLEMENTED_STUB(void, syr)
MAKE_UNIMPLEMENTED_STUB(void, syr2)

// lapack
MAKE_UNIMPLEMENTED_STUB(int, potrf)
MAKE_UNIMPLEMENTED_STUB(int, potri)
#undef MAKE_UNIMPLEMENTED_STUB

//------------------------------------------------------------------------------
// is_blas_scalar_v
//------------------------------------------------------------------------------
template <class T>
constexpr bool is_blas_scalar_v =
    std::is_same_v<uncvref_t<T>, float> || std::is_same_v<uncvref_t<T>, double>;
} // namespace satyr

#ifdef SATYR_WITH_MKL
#include <satyr/mkl.h>
#endif
