#pragma once

#include <cstddef>
#include <type_traits>

namespace satyr::detail {
//------------------------------------------------------------------------------
// type_list
//------------------------------------------------------------------------------
template <class... Tx>
struct type_list;

//------------------------------------------------------------------------------
// is_type_list_v
//------------------------------------------------------------------------------
template <class> struct is_type_list_impl {
  static constexpr bool value = false;
};

template <class... Tx> struct is_type_list_impl<type_list<Tx...>> {
  static constexpr bool value = true;
};

template <class T>
constexpr bool is_type_list_v = is_type_list_impl<T>::value;

//------------------------------------------------------------------------------
// is_unique_type_list_v
//------------------------------------------------------------------------------
template <class> struct is_unique_type_list_impl {
  static constexpr bool value = false;
};

template <> struct is_unique_type_list_impl<type_list<>> {
  static constexpr bool value = true;
};

// clang-format off
template <class T1, class... TRest>
  requires !(std::is_same_v<T1, TRest> || ...)
struct is_unique_type_list_impl<type_list<T1, TRest...>> {
  // clang-format on
  static constexpr bool value =
      is_unique_type_list_impl<type_list<TRest...>>::value;
};

template <class T>
constexpr bool is_unique_type_list_v = is_unique_type_list_impl<T>::value;

//------------------------------------------------------------------------------
// type_list_size_v
//------------------------------------------------------------------------------
template <class> constexpr size_t type_list_size_v;

template <class... Tx>
constexpr size_t type_list_size_v<type_list<Tx...>> = sizeof...(Tx);

//------------------------------------------------------------------------------
// type_list_element_t
//------------------------------------------------------------------------------
template <size_t, class> struct type_list_element_impl;

template <class ElementFirst, class... ElementsRest>
struct type_list_element_impl<0, type_list<ElementFirst, ElementsRest...>> {
  using type = ElementFirst;
};

template <size_t I, class ElementFirst, class... ElementsRest>
struct type_list_element_impl<I, type_list<ElementFirst, ElementsRest...>> {
  using type =
      typename type_list_element_impl<I - 1, type_list<ElementsRest...>>::type;
};

// clang-format off
template <size_t I, class TypeList>
  requires is_type_list_v<TypeList> && I < type_list_size_v<TypeList>
using type_list_element_t = 
    typename type_list_element_impl<I, TypeList>::type;
// clang-format on
}  // namespace echo::detail
