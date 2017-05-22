#pragma once


#include <iostream>
#include <satyr/k_array/k_array_view.h>
#include <satyr/k_array/k_array_subview.h>

namespace satyr {
//------------------------------------------------------------------------------
// print_impl
//------------------------------------------------------------------------------
namespace detail {
template <size_t K, class Accessor>
void print_impl(std::ostream& out, const std::array<index_t, K>& extents,
  const Accessor& accessor) {
  if constexpr (K == 0) {
    out << accessor();
  } else {
    out << '{';
    for (index_t index=0; index<extents[0]; ++index) {
      print_impl(out,
                 *reinterpret_cast<const std::array<index_t, K - 1>*>(
                     extents.data() + 1),
                 [&](auto... indexes_rest) {
                   return accessor(index, indexes_rest...);
                 });
      if (index != extents[0]-1)
        out << ", ";
    }
    out << '}';
  }
}
} // namespace detail

//------------------------------------------------------------------------------
// operator<<
//------------------------------------------------------------------------------
template <class T, size_t K>
  requires requires (std::ostream& out, T t) {
    out << t;
  }
std::ostream& operator<<(std::ostream& out, const k_array_view<T, K>& array) {
  detail::print_impl(out, array.shape().extents(), array);
  return out;
}

template <class T, size_t K>
  requires requires (std::ostream& out, T t) {
    out << t;
  }
std::ostream& operator<<(std::ostream& out, const k_array_subview<T, K>& array) {
  detail::print_impl(out, array.shape().extents(), array);
  return out;
}
} // namespace satyr
