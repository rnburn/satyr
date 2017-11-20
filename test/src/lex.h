#pragma once

#include "token_stream.h"

namespace satyr::testing {
//------------------------------------------------------------------------------
// lex
//------------------------------------------------------------------------------
token_stream lex(std::string_view s);
} // namespace satyr::testing
