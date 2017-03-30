#pragma once

namespace satyr {
//------------------------------------------------------------------------------
// Functor
//------------------------------------------------------------------------------
namespace detail {
template <class T, class U>
constexpr bool is_functor_impl = false;

template <class Return, class... Args, class F>
  requires requires(Args... args, F f) {
    { f(args...) } -> Return;
  }
constexpr bool is_functor_impl<Return(Args...), F> = true;
}

template <class T, class F>
concept bool Functor = detail::is_functor_impl<T, F>; 
} // namespace satyr
