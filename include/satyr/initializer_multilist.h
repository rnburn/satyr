#pragma once

#include <satyr/index.h>
#include <initializer_list>
#include <cstddef>
#include <array>

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
// get_extents
//------------------------------------------------------------------------------
template <class T, size_t N>
std::array<index_t, N> get_extents(initializer_multilist<T, N> values) {
  std::array<index_t, N> extents = {};
  extents[0] = values.size();
  if constexpr (N > 1) {
    for (auto value : values) {
      auto extents_rest = get_extents<T, N-1>(value);
      for (index_t i=0; i<N-1; ++i)
        extents[i+1] = std::max(extents[i+1], extents_rest[i]);
    }
  }
  return extents;
}

//------------------------------------------------------------------------------
// initialize
//------------------------------------------------------------------------------
template <class T, size_t N, class F>
void initialize(initializer_multilist<T, N> values, F f) {
  if constexpr(N == 0) {
    f() = values;
  } else {
    index_t index = 0;
    for (auto value : values) {
      initialize<T, N - 1>(
          value, [&](auto... args) -> auto& { return f(index, args...); });
      ++index;
    }
  }
}
} // namespace satyr::detail
