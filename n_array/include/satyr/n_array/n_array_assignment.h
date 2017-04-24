#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/map.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_assignment
//------------------------------------------------------------------------------
template <class Derived>
struct n_array_assignment {
  template <NArrayExpressible Rhs>
    requires detail::have_common_structure_v<Rhs> && 
             detail::have_common_shape_v<Rhs> &&
             detail::have_common_evaluator_v<Rhs>
  Derived& operator=(const Rhs& rhs) {
    auto f = [](auto& lhs, auto rhs) {
      return lhs = rhs;
    };
    return static_cast<Derived&>(*this);
  }
};
} // namespace satyr
