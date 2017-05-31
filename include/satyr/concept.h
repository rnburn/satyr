#pragma once

#include <type_traits>
#include <utility>
#include <complex>
#include <satyr/index.h>
#include <satyr/traits.h>

namespace satyr {
//------------------------------------------------------------------------------
// RealScalar
//------------------------------------------------------------------------------
template <class T>
concept bool RealScalar = std::is_integral_v<uncvref_t<T>> ||
                          std::is_same_v<uncvref_t<T>, float> ||
                          std::is_same_v<uncvref_t<T>, double>;

//------------------------------------------------------------------------------
// Scalar
//------------------------------------------------------------------------------
template <class T>
concept bool Scalar =
    RealScalar<T> || std::is_same_v<uncvref_t<T>, std::complex<float>> ||
    std::is_same_v<uncvref_t<T>, std::complex<double>>;

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
template <class, class F>
constexpr bool is_index_functor_impl = false;

template <size_t... Indexes, class F>
  requires requires(F f, std::enable_if_t<(Indexes,true), index_t>... indexes) {
    f(indexes...);
  }
constexpr bool
    is_index_functor_impl<std::index_sequence<Indexes...>, F> = true;
} // namespace detail

template <class F, size_t K>
concept bool IndexFunctor =
    detail::is_index_functor_impl<std::make_index_sequence<K>, F>;
}  // namespace satyr
