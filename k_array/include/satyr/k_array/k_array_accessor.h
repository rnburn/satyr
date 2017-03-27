#pragma once

#include <satyr/index.h>
#include <satyr/k_array/shape.h>
#include <satyr/k_array/subshape.h>
#include <satyr/k_array/traits.h>
#include <utility>

namespace satyr::detail {
//------------------------------------------------------------------------------
// k_array_const_accessor
//------------------------------------------------------------------------------
template <class Indexes, class Derived>
struct k_array_const_accessor_impl;

template <size_t... Indexes, class Derived>
struct k_array_const_accessor_impl<std::index_sequence<Indexes...>, Derived> {
  decltype(auto) operator()(
      std::enable_if_t<(Indexes,true),index_t>... indexes) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return *(derived.data() + get_1d_index(derived.shape(), indexes...));
  }
};

template <class Derived, size_t K>
struct k_array_const_accessor
    : k_array_const_accessor_impl<
          std::make_index_sequence<K>, Derived> {};

//------------------------------------------------------------------------------
// k_array_accessor
//------------------------------------------------------------------------------
template <class Indexes, class Derived>
struct k_array_accessor_impl;

template <size_t... Indexes, class Derived>
struct k_array_accessor_impl<std::index_sequence<Indexes...>, Derived> {
  decltype(auto) operator()(
      std::enable_if_t<(Indexes,true),index_t>... indexes) {
    Derived& derived = static_cast<Derived&>(*this);
    return *(derived.data() + get_1d_index(derived.shape(), indexes...));
  }
};

template <class Derived, size_t K>
struct k_array_accessor
    : k_array_accessor_impl<
          std::make_index_sequence<K>, Derived> {};

} // namespace satyr::detail
