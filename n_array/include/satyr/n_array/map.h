#pragma once

#include <satyr/n_array/map_evaluator.h>
#include <satyr/n_array/evaluator.h>
#include <satyr/n_array/common_structure.h>
#include <satyr/n_array/n_array_expression.h>

namespace satyr {
//------------------------------------------------------------------------------
// map
//------------------------------------------------------------------------------
template <class Functor, class... Expressibles>
auto map(Functor f, Expressibles&&... expressibles) {
  using Structure = common_structure_t<Expressibles...>;
  auto shape = get_common_shape(expressibles...);
  constexpr size_t num_dimensions = num_dimensions_v<decltype(shape)>;

  /* auto evaluator = detail::make_map_evaluator( */
  /* ); */
}
} // namespace satyr
