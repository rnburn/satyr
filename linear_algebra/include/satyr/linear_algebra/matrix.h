#pragma once

#include <satyr/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// matrix
//------------------------------------------------------------------------------
template <Scalar Scalar>
using matrix = n_array<Scalar, 2, structure_general>;

//------------------------------------------------------------------------------
// matrix_view
//------------------------------------------------------------------------------
template <Scalar Scalar>
using matrix_view = n_array_view<Scalar, 2, structure_general>;

//------------------------------------------------------------------------------
// matrix_cview
//------------------------------------------------------------------------------
template <Scalar Scalar>
using matrix_cview = n_array_cview<Scalar, 2, structure_general>;
} // namespace satyr
