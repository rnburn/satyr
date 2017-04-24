#pragma once

#include <satyr/n_array/map.h>

namespace satyr {
#define SATYR_MAKE_OPERATOR(SYMBOL) \
template <Expressible Lhs, Expressible Rhs> \
  requires (NArrayExpressible<Lhs> || NArrayExpressible<Rhs>) && \
           detail::have_common_structure_v<Lhs, Rhs> && \
           detail::have_common_shape_v<Lhs, Rhs> && \
           detail::have_common_evaluator_v<Lhs, Rhs> \
auto operator SYMBOL(Lhs&& lhs, Rhs&& rhs) { \
  auto f = [](auto x, auto y) { \
    return x SYMBOL y; \
  }; \
  return map(f, lhs, rhs); \
}
SATYR_MAKE_OPERATOR(+)
SATYR_MAKE_OPERATOR(-)
SATYR_MAKE_OPERATOR(*)
SATYR_MAKE_OPERATOR(/)
SATYR_MAKE_OPERATOR(==)
SATYR_MAKE_OPERATOR(!=)
#undef SATYR_MAKE_OPERATOR

#define SATYR_MAKE_OPERATOR(SYMBOL) \
template <RealExpressible Lhs, RealExpressible Rhs> \
  requires (RealNArrayExpressible<Lhs> || RealNArrayExpressible<Rhs>) && \
           detail::have_common_structure_v<Lhs, Rhs> && \
           detail::have_common_shape_v<Lhs, Rhs> && \
           detail::have_common_evaluator_v<Lhs, Rhs> \
auto operator SYMBOL(Lhs&& lhs, Rhs&& rhs) { \
  auto f = [](auto x, auto y) { \
    return x SYMBOL y; \
  }; \
  return map(f, lhs, rhs); \
}
SATYR_MAKE_OPERATOR(<)
SATYR_MAKE_OPERATOR(<=)
SATYR_MAKE_OPERATOR(>)
SATYR_MAKE_OPERATOR(>=)
#undef SATYR_MAKE_OPERATOR

#define SATYR_MAKE_OPERATOR(SYMBOL) \
template <NArrayExpressible T> \
auto operator SYMBOL(T&& t) { \
  auto f = [](auto x) { \
    return SYMBOL x; \
  }; \
  return map(f, t); \
}
SATYR_MAKE_OPERATOR(-)
#undef SATYR_MAKE_OPERATOR
} // namespace satyr
