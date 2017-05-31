#pragma once

#include <satyr/preprocessor.h>
#include <exception>
#include <iostream>
#include <satyr/traits.h>

namespace satyr {
#define MAKE_UNIMPLEMENTED_STUB(NAME)                           \
  void NAME(...) {                                              \
    std::cerr << SATYR_STRINGIFY(NAME) " is not implemented\n"; \
    std::terminate();                                           \
  }
MAKE_UNIMPLEMENTED_STUB(dot)
MAKE_UNIMPLEMENTED_STUB(gemv)
MAKE_UNIMPLEMENTED_STUB(trsv)
MAKE_UNIMPLEMENTED_STUB(trmv)
MAKE_UNIMPLEMENTED_STUB(symv)
MAKE_UNIMPLEMENTED_STUB(syr)
MAKE_UNIMPLEMENTED_STUB(syr2)
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
