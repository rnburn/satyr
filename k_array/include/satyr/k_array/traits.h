#pragma once

#include <satyr/k_array/shape.h>
#include <satyr/k_array/subshape.h>

namespace satyr {
template <class T>
constexpr size_t num_dimensions_v;

template <size_t K>
constexpr size_t num_dimensions_v<shape<K>> = K;
} // namespace satyr
