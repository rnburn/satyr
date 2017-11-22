#pragma once

#include <satyr/linear_algebra/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// common_structure_type
//------------------------------------------------------------------------------
template <>
struct common_structure_type<lower_triangular_structure,
                             upper_triangular_structure> {
  using type = general_structure;
};

template <>
struct common_structure_type<upper_triangular_structure,
                             lower_triangular_structure> {
  using type = general_structure;
};

template <>
struct common_structure_type<symmetric_structure, lower_triangular_structure> {
  using type = general_structure;
};

template <>
struct common_structure_type<lower_triangular_structure, symmetric_structure> {
  using type = general_structure;
};

template <>
struct common_structure_type<symmetric_structure, upper_triangular_structure> {
  using type = general_structure;
};

template <>
struct common_structure_type<upper_triangular_structure, symmetric_structure> {
  using type = general_structure;
};
} // namespace satry
