#pragma once

#include <cmath>

namespace satyr {
#define MAKE_FUNCTIONAL(NAME, OPERATOR) \
struct NAME { \
  template <class X, class Y> \
  auto operator()(X& x, Y y) const -> decltype(x OPERATOR y) { \
    return x OPERATOR y; \
  } \
};
MAKE_FUNCTIONAL(equals_functor, =);
MAKE_FUNCTIONAL(plus_equals_functor, +=);
MAKE_FUNCTIONAL(minus_equals_functor, -=);
MAKE_FUNCTIONAL(times_equals_functor, *=);
MAKE_FUNCTIONAL(divides_equals_functor, /=);
#undef MAKE_FUNCTIONAL

#define MAKE_FUNCTIONAL(NAME, FUNCTION) \
struct NAME { \
  template <class X> \
  auto operator()(X x) const -> decltype(FUNCTION(x)) { \
    return FUNCTION(x); \
  } \
};
MAKE_FUNCTIONAL(sqrt_functor, std::sqrt)
MAKE_FUNCTIONAL(log_functor, std::log)
MAKE_FUNCTIONAL(exp_functor, std::exp)
MAKE_FUNCTIONAL(sin_functor, std::sin)
MAKE_FUNCTIONAL(cos_functor, std::cos)
MAKE_FUNCTIONAL(tan_functor, std::tan)
MAKE_FUNCTIONAL(abs_functor, std::abs)
#undef MAKE_FUNCTIONAL

struct square_functor {
  template <class X>
  auto operator()(X x) const -> decltype(x*x) {
    return x*x;
  }
};
} // namespace satyr
