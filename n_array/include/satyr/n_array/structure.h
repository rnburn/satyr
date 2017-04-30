#pragma once

#include <satyr/traits.h>

namespace satyr { 
struct structure {};

struct general_structure : structure {};

template <class T>
concept bool Structure = std::is_base_of_v<structure, T>;

template <class>
constexpr bool is_equal_dimensional_v = false;

template <class T>
  requires Structure<T> && requires {
    T::is_equal_dimensional;
  }
constexpr bool is_equal_dimensional_v<T> = true;

template <class T>
using structure_t = typename uncvref_t<T>::structure;

template <class T>
constexpr bool has_structure_v = false;

template <class T>
   requires requires {
     typename structure_t<T>;
   }
constexpr bool has_structure_v<T> = true;

template <Structure T>
struct refines_structure {};

template <Structure X, Structure Y>
constexpr bool refines_structure_v = std::is_base_of_v<refines_structure<Y>, X>;
} // namespace satyr
