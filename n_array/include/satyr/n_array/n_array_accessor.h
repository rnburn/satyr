#pragma once

#include <satyr/index.h>
#include <satyr/k_array.h>
#include <satyr/n_array/access_mode.h>
#include <satyr/n_array/structure.h>
#include <utility>

namespace satyr {
// fwd
template <class>
auto make_n_array_view();

namespace detail {
template <class>
auto deconstify_n_array_view();
}

//------------------------------------------------------------------------------
// n_array_const_accessor
//------------------------------------------------------------------------------
namespace detail {
template <class, class Derived, class Structure>
struct n_array_const_accessor_impl;

template <size_t... Indexes, class Derived, class Structure>
struct n_array_const_accessor_impl<std::index_sequence<Indexes...>, Derived,
                                   Structure> {
  const auto& operator()(access_mode::readonly, 
      std::enable_if_t<(Indexes,true), index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(indexes...);
  }

  auto& operator()(access_mode::readwrite, 
      std::enable_if_t<(Indexes,true), index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(indexes...);
  }

  auto& operator()(access_mode::raw, 
      std::enable_if_t<(Indexes,true), index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(indexes...);
  }

  decltype(auto) operator()(
      std::enable_if_t<(Indexes, true), index_t>... indexes) const {
    return this->operator()(access_mode::readwrite_v, indexes...);
  }

  template <class... Slices>
    requires sizeof...(Indexes) == sizeof...(Slices) &&
             (is_slice_v<Slices> && ...) &&
             has_free_slices_v<Slices...>
  auto operator()(Slices... slices) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    auto [shape_new, offset] = slice(derived.shape(), slices...);
    return make_n_array_view<general_structure>(derived.data() + offset,
                                                shape_new);
  }
};
} // namespace detail

template <class Derived, size_t K, Structure Structure>
struct n_array_const_accessor
    : detail::n_array_const_accessor_impl<std::make_index_sequence<K>, Derived,
                                          Structure> {};
//------------------------------------------------------------------------------
// deconstify_result
//------------------------------------------------------------------------------
namespace detail {
template <class T>
  requires std::is_same_v<const std::decay_t<T>&, T>
decltype(auto) deconstify_result(T&& t) {
  return const_cast<std::decay_t<T>&>(t);
}

template <class T>
  requires std::is_same_v<std::decay_t<T>&, T>
decltype(auto) deconstify_result(T&& t) {
  return t;
}

template <class T>
auto deconstify_result(T&& t) {
  return t;
}
}

//------------------------------------------------------------------------------
// n_array_accessor
//------------------------------------------------------------------------------
namespace detail {
template <class, class Derived, class Structure>
struct n_array_accessor_impl;

template <size_t... Indexes, class Derived, class Structure>
struct n_array_accessor_impl<std::index_sequence<Indexes...>, Derived,
                             Structure>
    : n_array_const_accessor<Derived, sizeof...(Indexes), Structure> {
    using base = n_array_const_accessor<Derived, sizeof...(Indexes), Structure>;
  public:
    using base::operator();

  auto& operator()(access_mode::readwrite, 
      std::enable_if_t<(Indexes,true), index_t>... indexes) {
    return deconstify_result(
        this->base::operator()(access_mode::readwrite_v, indexes...));
  }

  auto& operator()(access_mode::raw, 
      std::enable_if_t<(Indexes,true), index_t>... indexes) {
    return deconstify_result(
        this->base::operator()(access_mode::raw_v, indexes...));
  }

  decltype(auto) operator()(std::enable_if_t<(Indexes,true), index_t>... indexes) {
    return deconstify_result(
        this->base::operator()(access_mode::raw_v, indexes...));
  }

  template <class... Slices>
    requires sizeof...(Indexes) == sizeof...(Slices) &&
             (is_slice_v<Slices> && ...) &&
             has_free_slices_v<Slices...>
  auto operator()(Slices... slices) {
    return deconstify_n_array_view(this->base::operator()(slices...));
  }
};
} // namespace detail

template <class Derived, size_t K, Structure Structure>
struct n_array_accessor
    : detail::n_array_accessor_impl<std::make_index_sequence<K>, Derived,
                                    Structure> {};
} // namespace satyr
