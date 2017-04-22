#pragma once

#include <satyr/n_array/map_evaluator.h>
#include <satyr/n_array/evaluator.h>
#include <satyr/n_array/common_structure.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/conversion_evaluator.h>

namespace satyr {
//------------------------------------------------------------------------------
// map
//------------------------------------------------------------------------------
template <class Functor, class... Expressibles>
auto map(Functor f, Expressibles&&... expressibles) {
  using Structure = common_structure_t<Expressibles...>;
  auto shape = get_common_shape(expressibles...);
  constexpr size_t K = num_dimensions_v<decltype(shape)>;
  return make_n_array_expression<Structure>(
      shape,
      detail::make_map_evaluator<K>(
          f, convert_evaluator<Structure>(make_expression(expressibles...))));
}
} // namespace satyr
