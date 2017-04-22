#pragma once

#include <type_traits>
#include <memory>

namespace satyr {
//------------------------------------------------------------------------------
// reference_t
//------------------------------------------------------------------------------
template <class T> struct reference_type {};

// clang-format off
template <class T>
  requires requires { typename T::reference; }
struct reference_type<T> {
  // clang-format on
  using type = typename T::reference;
};

// clang-format off
template <class T>
  requires requires(T* t) { { *t } -> auto&&; }
struct reference_type<T*> {
  // clang-format on
  using type = T&;
};

// clang-format off
template <class T>
  requires requires(
      typename reference_type<std::remove_cv_t<T>>::type reference) {
    { reference } -> auto&&;
  }
using reference_t = typename reference_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// value_type_t
//------------------------------------------------------------------------------
template <class T> struct value_type {};

// clang-format off
template <class T>
  // requires DETAIL_NS::has_member_value_type_v<T>
  requires requires { typename T::value_type; }
struct value_type<T> {
  // clang-format on
  using type = typename T::value_type;
};

// clang-format off
template <class T>
  requires requires { typename T::element_type; }
struct value_type<T> {
  // clang-format on
  using type = typename T::element_type;
};

// clang-format off
template <class T>
  requires requires(T* t) { { *t } -> auto&&; }
struct value_type<T*> {
  // clang-format on
  using type = std::decay_t<T>;
};

// clang-format off
template <class T>
  requires requires(T x) { { x.data() } -> auto*; } &&
           !requires { typename T::value_type; } &&
           !requires { typename T::element_type; }
struct value_type<T> {
  // clang-format on
  using type = typename value_type<decltype(std::declval<T>().data())>::type;
};

// clang-format off
template <class T>
  requires requires(typename value_type<std::remove_cv_t<T>>::type value) {
    { value } -> auto&&;
  }
using value_type_t = typename value_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// pointer_t
//------------------------------------------------------------------------------
template <class> struct pointer_type {};

// clang-format off
template <class T>
  requires requires { typename T::pointer; }
struct pointer_type<T> {
  // clang-format on
  using type = typename T::pointer;
};

template <class T> struct pointer_type<T*> { using type = T*; };

// clang-format off
template <class T>
  requires requires { typename value_type_t<T>; } &&
           !requires { typename T::pointer; }
struct pointer_type<T> {
  // clang-format on
  using type = value_type_t<T>*;
};

// clang-format off
template <class T>
  requires requires { typename pointer_type<std::remove_cv_t<T>>::type; }
using pointer_t = typename pointer_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// const_pointer_t
//------------------------------------------------------------------------------
template <class> struct const_pointer_type {};

// clang-format off
template <class T>
  requires requires { typename T::const_pointer; }
struct const_pointer_type<T> {
  // clang-format on
  using type = typename T::const_pointer;
};

// clang-format off
template <class T>
  requires requires { 
              typename pointer_t<T>; 
              typename value_type_t<T>;
           } &&
           !requires { typename T::const_pointer; }
struct const_pointer_type<T> {
  // clang-format on
  using type = typename std::pointer_traits<pointer_t<T>>::template rebind<
      const value_type_t<T>>;
};

// clang-format off
template <class T>
  requires requires { typename const_pointer_type<std::remove_cv_t<T>>::type; }
using const_pointer_t = typename const_pointer_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// void_pointer_t
//------------------------------------------------------------------------------
template <class> struct void_pointer_type {};

// clang-format off
template <class T>
  requires requires { typename T::void_pointer; }
struct void_pointer_type<T> {
  // clang-format on
  using type = typename T::void_pointer;
};

// clang-format off
template <class T>
  requires requires { typename pointer_t<T>; } &&
           !requires { typename T::void_pointer; }
struct void_pointer_type<T> {
  // clang-format on
  using type =
      typename std::pointer_traits<pointer_t<T>>::template rebind<void>;
};

// clang-format off
template <class T>
  requires requires { typename void_pointer_type<std::remove_cv_t<T>>::type; }
using void_pointer_t = typename void_pointer_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// const_void_pointer_t
//------------------------------------------------------------------------------
template <class> struct const_void_pointer_type {};

// clang-format off
template <class T>
  requires requires { typename T::const_void_pointer; }
struct const_void_pointer_type<T> {
  // clang-format on
  using type = typename T::const_void_pointer;
};

// clang-format off
template <class T>
  requires requires { typename pointer_t<T>; } &&
           !requires { typename T::const_void_pointer; }
struct const_void_pointer_type<T> {
  // clang-format on
  using type =
      typename std::pointer_traits<pointer_t<T>>::template rebind<const void>;
};

// clang-format off
template <class T>
  requires requires { 
    typename const_void_pointer_type<std::remove_cv_t<T>>::type; 
  }
using const_void_pointer_t = 
  typename const_void_pointer_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// difference_type_t
//------------------------------------------------------------------------------
template <class> struct difference_type {};

// clang-format off
template <class T>
  requires requires { typename T::difference_type; }
struct difference_type<T> {
  // clang-format on
  using type = typename T::difference_type;
};

// clang-format off
template <class T>
  requires requires { typename pointer_t<T>; } &&
           !requires { typename T::difference_type; }
struct difference_type<T> {
  // clang-format on
  using type = typename std::pointer_traits<pointer_t<T>>::difference_type;
};

// clang-format off
template <class T>
  requires requires(
      typename difference_type<std::remove_cv_t<T>>::type difference)
  {
    requires std::is_integral<decltype(difference)>::value;
    requires std::is_signed<decltype(difference)>::value;
  }
using difference_type_t = typename difference_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// size_type_t
//------------------------------------------------------------------------------
template <class> struct size_type {};

// clang-format off
template <class T>
  requires requires { typename T::size_type; }
struct size_type<T> {
  // clang-format on
  using type = typename T::size_type;
};

// clang-format off
template <class T>
  requires requires { typename difference_type_t<T>; } &&
           !requires { typename T::size_type; }
struct size_type<T> {
  // clang-format on
  using type = std::make_unsigned_t<difference_type_t<T>>;
};

// clang-format off
template <class T>
  requires requires { typename size_type<std::remove_cv_t<T>>::type; }
using size_type_t = typename size_type<std::remove_cv_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// uncvref_t
//------------------------------------------------------------------------------
template <class T>
using uncvref_t = typename std::remove_cv_t<std::remove_reference_t<T>>;
} // namespace traits
