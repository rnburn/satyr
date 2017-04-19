#pragma once

#include <satyr/concept.h>
#include <satyr/k_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// first_num_dimensions_v
//------------------------------------------------------------------------------
namespace detail {
template <class...>
constexpr size_t first_num_dimensions_v = 0;

template <class TFirst, class... TRest>
  requires requires {
    typename shape_t<TFirst>;
  }
constexpr size_t first_num_dimensions_v<TFirst, TRest...> = 
                        num_dimensions_v<TFirst>;

template <class TFirst, class... TRest>
  requires !requires {
    typename shape_t<TFirst>;
  }
constexpr size_t first_num_dimensions_v<TFirst, TRest...> = 
                        first_num_dimensions_v<TRest...>;
} // namespace detail

//------------------------------------------------------------------------------
// match_num_dimensions_v
//------------------------------------------------------------------------------
namespace detail {
template <size_t K, class...>
constexpr size_t match_num_dimensions_v = false;

template <size_t K>
constexpr size_t match_num_dimensions_v<K> = true;

template <size_t K, class TFirst, class... TRest>
  requires requires {
    typename shape_t<TFirst>;
    requires num_dimensions_v<TFirst> == K;
  }
constexpr size_t match_num_dimensions_v<K, TFirst, TRest...> = 
                      match_num_dimensions_v<K, TRest...>;

template <size_t K, class TFirst, class... TRest>
  requires !requires {
    typename shape_t<TFirst>;
  }
constexpr size_t match_num_dimensions_v<K, TFirst, TRest...> = 
                      match_num_dimensions_v<K, TRest...>;
} // namespace detail

//------------------------------------------------------------------------------
// get_common_shape
//------------------------------------------------------------------------------
namespace detail {
template <class TFirst, class... TRest>
  requires requires {
    typename shape_t<TFirst>;
  }
auto get_common_shape_impl(const TFirst& t_first, const TRest&... t_rest) {
  return t_first.shape();
}

template <class TFirst, class... TRest>
  requires !requires {
    typename shape_t<TFirst>;
  }
auto get_common_shape_impl(const TFirst& t_first, const TRest&... t_rest) {
  return get_common_shape_impl(t_rest...);
}
} // namespace detail

template <class... Tx>
  requires detail::first_num_dimensions_v<Tx...> > 0 &&
           detail::match_num_dimensions_v<
                      detail::first_num_dimensions_v<Tx...>, Tx...>
auto get_common_shape(const Tx&... tx) {
  return detail::get_common_shape_impl(tx...);
}
} // namespace satyr
