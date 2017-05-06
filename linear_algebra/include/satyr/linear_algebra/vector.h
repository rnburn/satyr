#pragma once

#include <satyr/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// vector
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector = n_array<Scalar, 1, structure_general>;

//------------------------------------------------------------------------------
// vector_view
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector_view = n_array_view<Scalar, 1, structure_general>;

//------------------------------------------------------------------------------
// vector_cview
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector_cview = n_array_cview<Scalar, 1, structure_general>;
} // namespace satyr
