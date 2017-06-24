#pragma once

#include <satyr/concept.h>
#include <jemalloc/jemalloc.h>
#include <new>
#include <cassert>

namespace satyr {
//------------------------------------------------------------------------------
// scalar_allocate
//------------------------------------------------------------------------------
template <Scalar T>
T* scalar_allocate(index_t n) {
  assert(n);
  auto result = mallocx(n*sizeof(T), MALLOCX_ALIGN(64));
  if (!result)
    throw std::bad_alloc{};
  return static_cast<T*>(result);
}

//------------------------------------------------------------------------------
// scalar_dealloc
//------------------------------------------------------------------------------
template <Scalar T>
void scalar_deallocate(T* p) {
  assert(p);
  free(p);
}

//------------------------------------------------------------------------------
// scalar_reallocate
//------------------------------------------------------------------------------
template <Scalar T>
T* scalar_reallocate(T* p, index_t n) {
  assert(p);
  auto result = rallocx(p, n*sizeof(T), MALLOCX_ALIGN(64));
  if (!result)
    throw std::bad_alloc{};
  return static_cast<T*>(result);
}
} // namespace satyr
