#pragma once

#include <satyr/concept.h>
#include <cstdlib>
#include <new>

namespace satyr {
//------------------------------------------------------------------------------
// scalar_allocate
//------------------------------------------------------------------------------
template <Scalar T>
T* scalar_allocate(index_t n) {
  void* p;
  int rcode = posix_memalign(&p, 64, n*sizeof(T));
  if (rcode)
    throw std::bad_alloc{};
  return static_cast<T*>(p);
}

//------------------------------------------------------------------------------
// scalar_dealloc
//------------------------------------------------------------------------------
template <Scalar T>
void scalar_deallocate(T* p) {
  std::free(p);
}

//------------------------------------------------------------------------------
// scalar_reallocate
//------------------------------------------------------------------------------
template <Scalar T>
T* scalar_reallocate(T* p, index_t n) {
  std::free(p);
  return scalar_allocate<T>(n);
}
} // namespace satyr
