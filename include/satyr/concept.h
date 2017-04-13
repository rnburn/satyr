#pragma once

#include <type_traits>
#include <utility>
#include <satyr/index.h>

namespace satyr {
//------------------------------------------------------------------------------
// Scalar
//------------------------------------------------------------------------------
template <class T>
concept bool Scalar = std::is_integral_v<T> || std::is_same_v<T, double> ||
                      std::is_same_v<T, float>;

//------------------------------------------------------------------------------
// Functor
//------------------------------------------------------------------------------
namespace detail {
template <class T, class U>
constexpr bool is_functor_impl = false;

template <class Return, class... Args, class F>
  requires requires(Args... args, F f) {
    { f(args...) } -> Return;
  } && std::is_copy_constructible_v<F>

constexpr bool is_functor_impl<Return(Args...), F> = true;
}

template <class F, class T>
concept bool Functor = detail::is_functor_impl<T, F>;

//------------------------------------------------------------------------------
// IndexFunctor
//------------------------------------------------------------------------------
namespace detail {
template <class, class Return, class F>
constexpr bool is_index_functor_impl = false;

template <size_t... Indexes, class Return, class F>
constexpr bool
    is_index_functor_impl<std::index_sequence<Indexes...>, Return, F> =
        Functor<F, Return(std::enable_if_t<(Indexes, true), index_t>...)>;
}

template <class F, class Return, size_t K>
concept bool IndexFunctor =
    detail::is_index_functor_impl<std::make_index_sequence<K>, Return, F>;
} // namespace satyr
