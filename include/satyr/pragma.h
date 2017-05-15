#pragma once

#include <satyr/preprocessor.h>

#ifdef __INTEL_COMPILER
#define SATYR_PRAGMA_NONTEMPORAL _Pragma("vector nontemporal")
#define SATYR_PRAGMA_SIMD _Pragma("simd")

#define SATYR_PRAGMA_SIMD_LINEAR1(X) _Pragma(SATYR_STRINGIFY_IMPL(simd linear X))
#define SATYR_PRAGMA_SIMD_LINEAR(X) SATYR_PRAGMA_SIMD_LINEAR1((X))

#define SATYR_PRAGMA_ALIGNED _Pragma("vector aligned")
#define SATYR_PRAGMA_SUGGEST_INLINE _Pragma("inline recursive")
#define SATYR_PRAGMA_FORCE_INLINE _Pragma("forceinline recursive")
#else
#define SATYR_PRAGMA_NONTEMPORAL
#define SATYR_PRAGMA_SIMD _Pragma("omp simd")

#define SATYR_PRAGMA_SIMD_LINEAR1(X) _Pragma(SATYR_STRINGIFY_IMPL(omp simd linear X))
#define SATYR_PRAGMA_SIMD_LINEAR(X) SATYR_PRAGMA_SIMD_LINEAR1((X))

#define SATYR_PRAGMA_ALIGNED
#define SATYR_PRAGMA_SUGGEST_INLINE
#define SATYR_PRAGMA_FORCE_INLINE
#endif
