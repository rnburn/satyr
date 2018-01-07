#pragma once

#include <satyr/n_array.h>
#include <satyr/linear_algebra/structure.h>

namespace satyr {
template <NArrayExpressible Expressible>
  requires std::is_same_v<structure_t<Expressible>, symmetric_structure>
value_type_t<Expressible> sum_elements(const Expressible& expressible) {
  auto multiplier = map_indexes<symmetric_structure>(
      expressible.shape(), [](index_t i, index_t j) { return 1 + (i != j); });
  auto expression = expressible * multiplier;
  auto reduction_expression =
      make_n_array_reduction_expression<structure_t<Expressible>,
                                        sum_reducer<value_type_t<Expressible>>>(
          expression.shape(), expression.evaluator(), expression.policy());
  return execute(reduction_expression);
}
} // namespace satyr
