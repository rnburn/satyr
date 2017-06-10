#pragma once

#include <satyr/n_array/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// structure_cast
//------------------------------------------------------------------------------
template <Structure ToStructure, Scalar T, size_t K, Structure FromStructure>
const n_array<T, K, ToStructure>& structure_cast(
    const n_array<T, K, FromStructure>& array) {
  return reinterpret_cast<const n_array<T, K, ToStructure>&>(array);
}

template <Structure ToStructure, Scalar T, size_t K, Structure FromStructure>
n_array<T, K, ToStructure>& structure_cast(
    n_array<T, K, FromStructure>& array) {
  return reinterpret_cast<n_array<T, K, ToStructure>&>(array);
}

template <Structure ToStructure, Scalar T, size_t K, Structure FromStructure>
n_array<T, K, ToStructure>&& structure_cast(
    n_array<T, K, FromStructure>&& array) {
  return reinterpret_cast<n_array<T, K, ToStructure>&&>(array);
}

template <Structure ToStructure, Scalar T, size_t K, Structure FromStructure>
const n_array_view<T, K, ToStructure>& structure_cast(
    const n_array_view<T, K, FromStructure>& array) {
  return reinterpret_cast<const n_array_view<T, K, ToStructure>&>(array);
}

template <Structure ToStructure, Scalar T, size_t K, Structure FromStructure>
const n_array_subview<T, K, ToStructure>& structure_cast(
    const n_array_subview<T, K, FromStructure>& array) {
  return reinterpret_cast<const n_array_subview<T, K, ToStructure>&>(array);
}
}  // namespace satyr
