#pragma once

#include <satyr/n_array/n_array_expression.h>

namespace satyr {
//------------------------------------------------------------------------------
// is_index_scalar_functor_v
//------------------------------------------------------------------------------
namespace detail {
template <class, class>
constexpr bool is_index_scalar_functor_impl = false;

template <size_t... Indexes, class F>
  requires requires (F f, std::enable_if_t<(Indexes,true), index_t>... indexes) 
  {
    requires Scalar<decltype(f(indexes...))>;
  }
constexpr bool
    is_index_scalar_functor_impl<std::index_sequence<Indexes...>, F> = true;

template <size_t K, class F>
constexpr bool is_index_scalar_functor_v =
    is_index_scalar_functor_impl<std::make_index_sequence<K>, F>;
}  // namespace detail

//------------------------------------------------------------------------------
// map_indexes_expression
//------------------------------------------------------------------------------
namespace detail {
} // namespace detail

//------------------------------------------------------------------------------
// map_indexes
//------------------------------------------------------------------------------

} // namespace satyr
