#pragma once

#include "token_stream.h"
#include <tuple>

namespace satyr::testing {
//------------------------------------------------------------------------------
// parse_array
//------------------------------------------------------------------------------
std::tuple<std::vector<double>, std::vector<size_t>>
parse_array(size_t num_dimensions, token_stream& stream);
} // namespace satyr::testing
