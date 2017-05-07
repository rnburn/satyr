#pragma once

namespace satyr {
#define MAKE_FUNCTIONAL(NAME, OPERATOR) \
struct NAME { \
  template <class X, class Y> \
  auto operator()(X& x, Y y) const -> decltype(x OPERATOR y) { \
    return x OPERATOR y; \
  } \
};
MAKE_FUNCTIONAL(equals, =);
MAKE_FUNCTIONAL(plus_equals, +=);
MAKE_FUNCTIONAL(minus_equals, -=);
MAKE_FUNCTIONAL(times_equals, *=);
MAKE_FUNCTIONAL(divides_equals, /=);
#undef MAKE_FUNCTIONAL
} // namespace satyr
