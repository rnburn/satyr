#pragma once

#include <satyr/k_array/shape.h>
#include <satyr/k_array/subshape.h>
#include <satyr/traits.h>

namespace satyr {
//------------------------------------------------------------------------------
// is_shape_v
//------------------------------------------------------------------------------
namespace detail {
template <class>
constexpr bool is_shape_impl = false;

template<size_t K>
constexpr bool is_shape_impl<shape<K>> = true;

template<size_t K>
constexpr bool is_shape_impl<subshape<K>> = true;
}

template <class T>
constexpr bool is_shape_v = detail::is_shape_impl<uncvref_t<T>>;

//------------------------------------------------------------------------------
// has_shape_v
//------------------------------------------------------------------------------
template <class T>
constexpr bool has_shape_v = false;

template <class T>
  requires requires(const T& t) {
    requires is_shape_v<decltype(t.shape())>
  }
constexpr bool has_shape_v<T> = true;

//------------------------------------------------------------------------------
// shape_t
//------------------------------------------------------------------------------
template <class T>
  requires has_shape_v<T>
using shape_t = uncvref_t<decltype(std::declval<const T&>().shape())>;

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
  requires has_shape_v<T>
constexpr size_t num_dimensions_v<T> = 
  num_dimensions_v<std::decay_t<decltype(std::declval<const T&>().shape())>>;
} // namespace satyr
