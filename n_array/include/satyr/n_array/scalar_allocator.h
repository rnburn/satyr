#pragma once

#include <tbb/cache_aligned_allocator.h>

namespace satyr:: detail {
template <class T>
using scalar_allocator = tbb::cache_aligned_allocator<T>;
} // namespace satyr::detail
