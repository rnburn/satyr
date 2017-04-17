#pragma once

#include <satyr/k_array/shape.h>
#include <satyr/k_array/subshape.h>

namespace satyr {
//------------------------------------------------------------------------------
// num_dimensions_v
//------------------------------------------------------------------------------
template <class T>
constexpr size_t num_dimensions_v;

template <size_t K>
constexpr size_t num_dimensions_v<shape<K>> = K;

template <size_t K>
constexpr size_t num_dimensions_v<subshape<K>> = K;

template <class T>
  requires requires (const T& t) {
    t.shape();
  }
constexpr size_t num_dimensions_v<T> = 
  num_dimensions_v<std::decay_t<decltype(std::declval<const T&>().shape())>>;
} // namespace satyr
