#pragma once

#include <satyr/index.h>
#include <initializer_list>
#include <cstddef>

namespace satyr::detail {
//------------------------------------------------------------------------------
// initializer_multilist
//------------------------------------------------------------------------------
template <class T, size_t N>
struct initializer_multilist_impl {
  using type = std::initializer_list<
      typename initializer_multilist_impl<T, N - 1>::type>;
};

template <class T>
struct initializer_multilist_impl<T, 0> {
  using type = T;
};

template <class T, size_t N>
using initializer_multilist = typename initializer_multilist_impl<T, N>::type;

//------------------------------------------------------------------------------
// initialize
//------------------------------------------------------------------------------
template <class T, size_t N, class F>
void initialize(initializer_multilist<T, N> values, F f) {
  if constexpr(N == 0) {
    f(values);
  } else {
    index_t index = 0;
    for (auto value : values) {
      initialize(values, [&](auto... args) { f(index, args...); });
      ++index;
    }
  }
}
} // namespace satyr::detail
