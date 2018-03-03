#pragma once


#include <iostream>
#include <satyr/k_array/k_array_view.h>
#include <satyr/k_array/k_array_subview.h>

namespace satyr {
//------------------------------------------------------------------------------
// print_impl
//------------------------------------------------------------------------------
namespace detail {
struct ArrayPrintFormat {
  const char* open_brace = "{";
  const char* close_brace = "}";
  const char* separator = ", ";
};

template <size_t K, class Accessor>
void print_impl(const ArrayPrintFormat& format, std::ostream& out,
                const std::array<index_t, K>& extents,
                const Accessor& accessor) {
  if constexpr (K == 0) {
    out << accessor();
  } else {
    out << format.open_brace;
    for (index_t index=0; index<extents[0]; ++index) {
      print_impl(format, out,
                 *reinterpret_cast<const std::array<index_t, K - 1>*>(
                     extents.data() + 1),
                 [&](auto... indexes_rest) {
                   return accessor(index, indexes_rest...);
                 });
      if (index != extents[0]-1)
        out << format.separator;
    }
    out << format.close_brace;
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
  detail::print_impl(detail::ArrayPrintFormat{}, out, array.shape().extents(),
                     array);
  return out;
}

template <class T, size_t K>
  requires requires (std::ostream& out, T t) {
    out << t;
  }
std::ostream& operator<<(std::ostream& out, const k_array_subview<T, K>& array) {
  detail::print_impl(detail::ArrayPrintFormat{}, out, array.shape().extents(),
                     array);
  return out;
}

//------------------------------------------------------------------------------
// print_json
//------------------------------------------------------------------------------
template <class T, size_t K>
  requires requires (std::ostream& out, T t) {
    out << t;
  }
void print_json(std::ostream& out, const k_array_view<T, K>& array) {
  detail::print_impl(detail::ArrayPrintFormat{"[", "]", ","}, out,
                     array.shape().extents(), array);
}

template <class T, size_t K>
  requires requires (std::ostream& out, T t) {
    out << t;
  }
void print_json(std::ostream& out, const k_array_subview<T, K>& array) {
  detail::print_impl(detail::ArrayPrintFormat{"[", "]", ","}, out,
                     array.shape().extents(), array);
}
} // namespace satyr
