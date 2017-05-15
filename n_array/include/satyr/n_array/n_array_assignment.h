#pragma once

#include <satyr/functional.h>
#include <satyr/n_array/apply.h>
#include <satyr/n_array/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_assignment
//------------------------------------------------------------------------------
template <class Derived, class Lhs>
struct n_array_assignment {
#define MAKE_ASSIGNMENT_OPERATOR(NAME, OPERATOR) \
  template <Expressible Rhs> \
    requires is_applicable_v<NAME, Lhs, Rhs> \
  Derived& operator OPERATOR (const Rhs& rhs) { \
    auto& derived = static_cast<Derived&>(*this); \
    apply(NAME{}, make_expression(derived), make_expression(rhs)); \
    return derived; \
  }
MAKE_ASSIGNMENT_OPERATOR(equals_functor, =)
MAKE_ASSIGNMENT_OPERATOR(plus_equals_functor, +=)
MAKE_ASSIGNMENT_OPERATOR(minus_equals_functor, -=)
MAKE_ASSIGNMENT_OPERATOR(times_equals_functor, *=)
MAKE_ASSIGNMENT_OPERATOR(divides_equals_functor, /=)
#undef MAKE_ASSIGNMENT_OPERATOR
};

//------------------------------------------------------------------------------
// n_array_const_assignment
//------------------------------------------------------------------------------
template <class Derived, class Lhs>
struct n_array_const_assignment {
#define MAKE_ASSIGNMENT_OPERATOR(NAME, OPERATOR) \
  template <Expressible Rhs> \
    requires is_applicable_v<NAME, Lhs, Rhs> \
  const Derived& operator OPERATOR (const Rhs& rhs) const { \
    auto& derived = static_cast<const Derived&>(*this); \
    apply(NAME{}, make_expression(derived), make_expression(rhs)); \
    return derived; \
  }
MAKE_ASSIGNMENT_OPERATOR(equals_functor, =)
MAKE_ASSIGNMENT_OPERATOR(plus_equals_functor, +=)
MAKE_ASSIGNMENT_OPERATOR(minus_equals_functor, -=)
MAKE_ASSIGNMENT_OPERATOR(times_equals_functor, *=)
MAKE_ASSIGNMENT_OPERATOR(divides_equals_functor, /=)
#undef MAKE_ASSIGNMENT_OPERATOR
};
} // namespace satyr
