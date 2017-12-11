#pragma once

#include <satyr/type_list.h>
#include <satyr/index.h>
#include <type_traits>
#include <memory>

namespace satyr {
//------------------------------------------------------------------------------
// uncvref_t
//------------------------------------------------------------------------------
template <class T>
using uncvref_t = typename std::remove_cv_t<std::remove_reference_t<T>>;

//------------------------------------------------------------------------------
// value_type_t
//------------------------------------------------------------------------------
namespace detail {
template <class T>
constexpr bool has_value_type_v = false;

template <class T>
  requires requires {
    typename T::value_type;
  }
constexpr bool has_value_type_v<T> = true;

template <class T>
constexpr bool has_element_type_v = false;

template <class T>
  requires requires {
    typename T::element_type;
  }
constexpr bool has_element_type_v<T> = true;
} // namespace detail

template <class T> struct value_type {};

template <class T>
  requires detail::has_value_type_v<T>
struct value_type<T> {
  using type = typename T::value_type;
};

template <class T>
  requires detail::has_element_type_v<T>
struct value_type<T> {
  using type = typename T::element_type;
};

template <class T>
  requires requires(T* t) { { *t } -> auto&&; }
struct value_type<T*> {
  using type = std::decay_t<T>;
};

template <class T>
  requires requires(T x) { { x.data() } -> auto*; } 
        && !detail::has_value_type_v<T> && !detail::has_element_type_v<T>
struct value_type<T> {
  using type = typename value_type<decltype(std::declval<T>().data())>::type;
};

// clang-format off
template <class T>
  requires requires(typename value_type<uncvref_t<T>>::type value) {
    { value } -> auto&&;
  }
using value_type_t = typename value_type<uncvref_t<T>>::type;
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
  requires requires { typename pointer_type<uncvref_t<T>>::type; }
using pointer_t = typename pointer_type<uncvref_t<T>>::type;
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
  requires requires { typename const_pointer_type<uncvref_t<T>>::type; }
using const_pointer_t = typename const_pointer_type<uncvref_t<T>>::type;
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
  requires requires { typename void_pointer_type<uncvref_t<T>>::type; }
using void_pointer_t = typename void_pointer_type<uncvref_t<T>>::type;
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
    typename const_void_pointer_type<uncvref_t<T>>::type; 
  }
using const_void_pointer_t = 
  typename const_void_pointer_type<uncvref_t<T>>::type;
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
      typename difference_type<uncvref_t<T>>::type difference)
  {
    requires std::is_integral<decltype(difference)>::value;
    requires std::is_signed<decltype(difference)>::value;
  }
using difference_type_t = typename difference_type<uncvref_t<T>>::type;
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
  requires requires { typename size_type<uncvref_t<T>>::type; }
using size_type_t = typename size_type<uncvref_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// is_writable_v
//------------------------------------------------------------------------------
template <class T>
  requires requires(T t) {
    { t.data() } -> auto*;
  }
constexpr bool is_writable_v = 
    std::is_lvalue_reference_v<decltype(*std::declval<T>().data())>;

//------------------------------------------------------------------------------
// arity_v
//------------------------------------------------------------------------------
namespace detail {
template <class> struct arity_impl {};

template <class Return, class... Args> struct arity_impl<Return(Args...)> {
  static constexpr size_t value = sizeof...(Args);
};

template <class Return, class... Args> struct arity_impl<Return (*)(Args...)> {
  static constexpr size_t value = sizeof...(Args);
};

template <class Return, class... Args> struct arity_impl<Return (&)(Args...)> {
  static constexpr size_t value = sizeof...(Args);
};

#define CVREF_QUALIFY(CV_QUALIFER, REF_QUALIFER)                       \
  template <class Return, class Record, class... Args>                 \
  struct arity_impl<Return (Record::*)(Args...) CV_QUALIFER REF_QUALIFER> { \
    static constexpr size_t value = sizeof...(Args);                   \
  };
#include <satyr/cvref_qualifer.def>
#undef CVREF_QUALIFY

// clang-format off
template <class T> 
  requires requires {
    { &T::operator() } -> auto&&;
  }
struct arity_impl<T> {
  static constexpr size_t value = arity_impl<decltype(&T::operator())>::value;
  // clang-format on
};
} // namespace detail

// clang-format off
template <class T>
  requires requires {
    detail::arity_impl<uncvref_t<T>>::value;
  }
static constexpr size_t arity_v = 
                          detail::arity_impl<uncvref_t<T>>::value;
// clang-format on

//------------------------------------------------------------------------------
// argument_t
//------------------------------------------------------------------------------
template <size_t, class> struct argument_type {};

// clang-format off
template <size_t I, class Return, class... Args>
  requires I < sizeof...(Args)
struct argument_type<I, Return (Args...)> {
  // clang-format on
  using type = detail::type_list_element_t<I, detail::type_list<Args...>>;
};

// clang-format off
template <size_t I, class Return, class... Args>
  requires I < sizeof...(Args)
struct argument_type<I, Return (*)(Args...)> {
  // clang-format on
  using type = detail::type_list_element_t<I, detail::type_list<Args...>>;
};

// clang-format off
template <size_t I, class Return, class... Args>
  requires I < sizeof...(Args)
struct argument_type<I, Return (&)(Args...)> {
  // clang-format on
  using type = detail::type_list_element_t<I, detail::type_list<Args...>>;
};

// clang-format off
#define CVREF_QUALIFY(CV_QUALIFER, REF_QUALIFER)                               \
  template <size_t I, class Return, class Record, class... Args>               \
    requires I < sizeof...(Args)                                               \
  struct argument_type<I,                                                      \
                       Return (Record::*)(Args...) CV_QUALIFER REF_QUALIFER> { \
    using type = detail::type_list_element_t<I, detail::type_list<Args...>>;                    \
  };
#include <satyr/cvref_qualifer.def>
#undef CVREF_QUALIFY
// clang-format on

// clang-format off
template <size_t I, class T>
  requires requires { { &T::operator() } -> auto&&; } && I < arity_v<T>
struct argument_type<I, T> {
  // clang-format on
  using type = typename argument_type<I, decltype(&T::operator())>::type;
};

// clang-format off
template <size_t I, class T>
  requires I < arity_v<T>
using argument_t = typename argument_type<I, uncvref_t<T>>::type;
// clang-format on

//------------------------------------------------------------------------------
// codomain_t
//------------------------------------------------------------------------------
template <class> struct codomain_type {};

template <class Return, class... Args> struct codomain_type<Return(Args...)> {
  using type = Return;
};

template <class Return, class... Args>
struct codomain_type<Return (*)(Args...)> {
  using type = Return;
};

template <class Return, class... Args>
struct codomain_type<Return (&)(Args...)> {
  using type = Return;
};

#define CVREF_QUALIFY(CV_QUALIFER, REF_QUALIFER)                               \
  template <class Return, class Record, class... Args>                         \
  struct codomain_type<Return (Record::*)(Args...) CV_QUALIFER REF_QUALIFER> { \
    using type = Return;                                                       \
  };
#include <satyr/cvref_qualifer.def>
#undef CVREF_QUALIFY

template <class T> requires requires {
  { &T::operator() } ->auto&&;
}
struct codomain_type<T> {
  using type = typename codomain_type<decltype(&T::operator())>::type;
};

template <class T>
using codomain_t = typename codomain_type<uncvref_t<T>>::type;

//------------------------------------------------------------------------------
// domain_t
//------------------------------------------------------------------------------
template <class> struct domain_type {};

// clang-format off
template <class Return, class ArgFirst, class... ArgsRest>
  requires (std::is_same_v<ArgFirst, ArgsRest> && ...)
struct domain_type<Return (ArgFirst, ArgsRest...)> {
  // clang-format on
  using type = ArgFirst;
};

// clang-format off
template <class Return, class ArgFirst, class... ArgsRest>
  requires (std::is_same_v<ArgFirst, ArgsRest> && ...)
struct domain_type<Return (*)(ArgFirst, ArgsRest...)> {
  // clang-format on
  using type = ArgFirst;
};

// clang-format off
template <class Return, class ArgFirst, class... ArgsRest>
  requires (std::is_same_v<ArgFirst, ArgsRest> && ...)
struct domain_type<Return (&)(ArgFirst, ArgsRest...)> {
  // clang-format on
  using type = ArgFirst;
};

// clang-format off
#define CVREF_QUALIFY(CV_QUALIFER, REF_QUALIFER)                           \
  template <class Return, class Record, class ArgFirst, class... ArgsRest> \
    requires (std::is_same_v<ArgFirst, ArgsRest> && ...) \
  struct domain_type<Return (Record::*)(ArgFirst, ArgsRest...)             \
                         CV_QUALIFER REF_QUALIFER> {                       \
    using type = ArgFirst;                                                 \
  };
#include <satyr/cvref_qualifer.def>
#undef CVREF_QUALIFY
// clang-format on

template <class T> 
  requires requires {
    { &T::operator() } ->auto&&;
  }
struct domain_type<T> {
  using type = typename domain_type<decltype(&T::operator())>::type;
};

template <class T>
using domain_t = typename domain_type<uncvref_t<T>>::type;

//------------------------------------------------------------------------------
// codomain_value_type_t
//------------------------------------------------------------------------------
template <class T>
using codomain_value_type_t =
    uncvref_t<std::remove_reference_t<codomain_t<T>>>;

//------------------------------------------------------------------------------
// domain_value_type_t
//------------------------------------------------------------------------------
template <class T>
using domain_value_type_t =
    uncvref_t<std::remove_reference_t<domain_t<T>>>;

//------------------------------------------------------------------------------
// index_functor_codomain_t
//------------------------------------------------------------------------------
namespace detail {
template <class, class>
struct index_functor_codomain_type;

template <size_t... Indexes, class F>
struct index_functor_codomain_type<std::index_sequence<Indexes...>, F> {
  using type = decltype(std::declval<F>()(((void)Indexes, index_t{0})...));
};
} // namespace detail

template <class F, size_t K>
using index_functor_codomain_t =
    typename detail::index_functor_codomain_type<std::make_index_sequence<K>,
                                                 F>::type;

//------------------------------------------------------------------------------
// index_functor_codomain_value_type_t
//------------------------------------------------------------------------------
template <class F, size_t K>
using index_functor_codomain_value_type_t =
    uncvref_t<index_functor_codomain_t<F, K>>;

//------------------------------------------------------------------------------
// is_same_v
//------------------------------------------------------------------------------
template <class TFirst, class TSecond, class... TRest>
constexpr bool is_same_v = std::is_same_v<TFirst, TSecond> &&
                           (std::is_same_v<TFirst, TRest> && ...);
}  // namespace traits
