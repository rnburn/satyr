#pragma once

#include <type_traits>

namespace satyr { 
struct structure {};

struct general_structure : structure {};

template <class>
constexpr bool is_equal_dimensional_v = false;

template <class T>
  requires std::is_base_of_v<structure, T> && requires {
    T::is_equal_dimensional;
  }
constexpr bool is_equal_dimensional_v<T> = true;
} // namespace satyr
