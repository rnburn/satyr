#pragma once

#include <satyr/k_array/shape.h>

namespace satyr {
//------------------------------------------------------------------------------
// subshape
//------------------------------------------------------------------------------
template <size_t K>
class subshape {
 public:
 private:
   shape<K> shape_;
   std::array<index_t, K> strides_;
};
} // namespace satyr
