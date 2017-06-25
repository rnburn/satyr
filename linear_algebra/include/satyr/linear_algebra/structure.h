#pragma once

#include <satyr/n_array.h>
#include <satyr/matrix.h>

namespace satyr {
//------------------------------------------------------------------------------
// triangular_structure
//------------------------------------------------------------------------------
template <uplo_t Uplo>
struct triangular_structure : structure, refines_structure<general_structure> {
  static constexpr bool is_equal_dimensional = true;
  static constexpr uplo_t uplo = Uplo;
};

using upper_triangular_structure = triangular_structure<uplo_t::upper>;
using lower_triangular_structure = triangular_structure<uplo_t::lower>;

//------------------------------------------------------------------------------
// symmetric_structure
//------------------------------------------------------------------------------
struct symmetric_structure : structure, refines_structure<general_structure> {
  static constexpr bool is_equal_dimensional = true;
  static constexpr uplo_t uplo = uplo_t::lower;
};

//------------------------------------------------------------------------------
// diagonal_structure
//------------------------------------------------------------------------------
struct diagonal_structure : structure,
                            refines_structure<general_structure>,
                            refines_structure<upper_triangular_structure>,
                            refines_structure<lower_triangular_structure>,
                            refines_structure<symmetric_structure> {
  static constexpr bool is_equal_dimensional = true;
};
}  // namespace satyr
