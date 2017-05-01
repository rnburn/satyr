#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/map.h>
#include <satyr/n_array/execute.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_assignment
//------------------------------------------------------------------------------
template <class Derived, class Lhs>
struct n_array_assignment {
#define MAKE_ASSIGNMENT_OPERATOR(OPERATOR) \
  template <Expressible Rhs> \
    requires detail::have_common_structure_v<Lhs, Rhs> && \
             detail::have_common_shape_v<Lhs, Rhs> && \
             detail::have_common_evaluator_v<Lhs, Rhs> \
  Derived& operator OPERATOR (const Rhs& rhs) { \
    auto f = [](auto& lhs, auto rhs) -> decltype(lhs OPERATOR rhs) { \
      return lhs OPERATOR rhs; \
    }; \
    auto& derived = static_cast<Derived&>(*this); \
    execute(detail::map_impl(f, derived, rhs)); \
    return derived; \
  }
MAKE_ASSIGNMENT_OPERATOR(=)
MAKE_ASSIGNMENT_OPERATOR(+=)
MAKE_ASSIGNMENT_OPERATOR(-=)
MAKE_ASSIGNMENT_OPERATOR(*=)
MAKE_ASSIGNMENT_OPERATOR(/=)
#undef MAKE_ASSIGNMENT_OPERATOR
};

//------------------------------------------------------------------------------
// n_array_const_assignment
//------------------------------------------------------------------------------
template <class Derived, class Lhs>
struct n_array_const_assignment {
#define MAKE_ASSIGNMENT_OPERATOR(OPERATOR) \
  template <Expressible Rhs> \
    requires detail::have_common_structure_v<Lhs, Rhs> && \
             detail::have_common_shape_v<Lhs, Rhs> && \
             detail::have_common_evaluator_v<Lhs, Rhs> \
  Derived& operator OPERATOR (const Rhs& rhs) const { \
    auto f = [](auto& lhs, auto rhs) -> decltype(lhs OPERATOR rhs) { \
      return lhs OPERATOR rhs; \
    }; \
    auto& derived = static_cast<const Derived&>(*this); \
    execute(detail::map_impl(f, derived, rhs)); \
    return derived; \
  }
MAKE_ASSIGNMENT_OPERATOR(=)
MAKE_ASSIGNMENT_OPERATOR(+=)
MAKE_ASSIGNMENT_OPERATOR(-=)
MAKE_ASSIGNMENT_OPERATOR(*=)
MAKE_ASSIGNMENT_OPERATOR(/=)
#undef MAKE_ASSIGNMENT_OPERATOR
};
} // namespace satyr
