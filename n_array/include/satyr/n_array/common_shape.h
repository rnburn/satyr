#pragma once

#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// get_common_shape
//------------------------------------------------------------------------------
namespace detail {
template <size_t, class...>
struct match_shapes {
  static constexpr bool value = true;
};

template <size_t K, Scalar Scalar, class... Rest>
struct match_shapes<K, Scalar, Rest...>
  : match_shapes<K, Rest...>
{};


}
} // namespace satyr
