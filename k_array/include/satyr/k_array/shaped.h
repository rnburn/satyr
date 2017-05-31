#pragma once

#include <satyr/k_array/traits.h>
#include <satyr/k_array/shape.h>

namespace satyr {
//------------------------------------------------------------------------------
// get_extent
//------------------------------------------------------------------------------
template <size_t I, class T>
  requires has_shape_v<T> &&
           I < num_dimensions_v<T>
index_t get_extent(const T& t) {
  return get_extent<I>(t.shape());
}

//------------------------------------------------------------------------------
// get_stride
//------------------------------------------------------------------------------
template <size_t I, class T>
  requires has_shape_v<T> &&
           I < num_dimensions_v<T>
index_t get_stride(const T& t) {
  return get_stride<I>(t.shape());
}

//------------------------------------------------------------------------------
// get_num_elements
//------------------------------------------------------------------------------
template <class T>
  requires has_shape_v<T>
index_t get_num_elements(const T& t) {
  return get_num_elements(t.shape());
}
} // namespace satyr
