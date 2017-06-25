#pragma once

#include <satyr/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// vector
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector = n_array<Scalar, 1, general_structure>;

//------------------------------------------------------------------------------
// vector_view
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector_view = n_array_view<Scalar, 1, general_structure>;

//------------------------------------------------------------------------------
// vector_cview
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector_cview = n_array_cview<Scalar, 1, general_structure>;

//------------------------------------------------------------------------------
// vector_subview
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector_subview = n_array_subview<Scalar, 1, general_structure>;

//------------------------------------------------------------------------------
// vector_cview
//------------------------------------------------------------------------------
template <Scalar Scalar>
using vector_subcview = n_array_subcview<Scalar, 1, general_structure>;
} // namespace satyr
