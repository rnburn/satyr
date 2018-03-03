#pragma once

#include <satyr/n_array/n_array_view.h>
#include <satyr/n_array/n_array_subview.h>

namespace satyr {
//------------------------------------------------------------------------------
// operatator<<
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
std::ostream& operator<<(std::ostream& out,
                         const n_array_view<T, K, Structure>& array) {
  detail::print_impl(detail::ArrayPrintFormat{}, out, array.shape().extents(),
                     array);
  return out;
}

template <Scalar T, size_t K, Structure Structure>
std::ostream& operator<<(std::ostream& out,
                         const n_array_subview<T, K, Structure>& array) {
  detail::print_impl(detail::ArrayPrintFormat{}, out, array.shape().extents(),
                     array);
  return out;
}

//------------------------------------------------------------------------------
// print_json
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
void print_json(std::ostream& out, const n_array_view<T, K, Structure>& array) {
  detail::print_impl(detail::ArrayPrintFormat{"[", "]", ","}, out,
                     array.shape().extents(), array);
}

template <Scalar T, size_t K, Structure Structure>
void print_json(std::ostream& out,
                const n_array_subview<T, K, Structure>& array) {
  detail::print_impl(detail::ArrayPrintFormat{"[", "]", ","}, out,
                     array.shape().extents(), array);
}
} // namespace satyr
