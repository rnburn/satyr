#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// common_structure_type
//------------------------------------------------------------------------------
template <class, class> struct common_structure_type {};

template <Structure T> struct common_structure_type<T, T> {
  using type = T;
};

template <Structure X, Structure Y>
  requires std::is_base_of_v<X, Y>
struct common_structure_type<X, Y> {
  using type = X;
};

template <Structure X, Structure Y>
  requires std::is_base_of_v<Y, X>
struct common_structure_type<X, Y> {
  using type = Y;
};

//------------------------------------------------------------------------------
// common_structure_t
//------------------------------------------------------------------------------
namespace detail {
struct null_structure {};

template <class...> struct common_structure_impl {};

template <Structure T> struct common_structure_impl<T> {
  using type = T;
};

template <Structure X, Structure Y, class... Rest>
struct common_structure_impl<X, Y, Rest...>
    : common_structure_impl<typename common_structure_type<X, Y>::type,
                            Rest...> {};

template <class... Rest>
struct common_structure_impl<null_structure, Rest...>
  : common_structure_impl<Rest...> {};

template <class T, class... Rest>
struct common_structure_impl<T, null_structure, Rest...> :
  common_structure_impl<T, Rest...> {};

template <class T> struct structure_or_null {
  using type = null_structure;
};

template <Structure T> struct structure_or_null<T> {
  using type = typename T::structure;
};

template <class... Tx>
  requires ((has_structure_v<Tx> || Scalar<Tx>) && ...) &&
           (has_structure_v<Tx> + ... ) > 0
using common_structure_t =
  typename detail::common_structure_impl<Tx...>::type;
} // namespace detail
} // namespace satyr
