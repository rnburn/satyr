#pragma once

#include <satyr/linear_algebra/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// apply
//------------------------------------------------------------------------------
template <class Functor, Evaluator<2> Evaluator1, Policy Policy1,
          Structure RhsStructure, KEvaluator<2> Evaluator2, Policy Policy2>
  requires (std::is_same_v<RhsStructure, lower_triangular_structure> ||
            std::is_same_v<RhsStructure, symmetric_structure> ||
            std::is_same_v<RhsStructure, upper_triangular_structure>) &&
           detail::is_mappable_v<
                Functor,
                n_array_expression<2, general_structure, Evaluator1, Policy1>,
                n_array_expression<2, RhsStructure, Evaluator2, Policy2>>
void apply(
    Functor f,
    const n_array_expression<2, general_structure, Evaluator1, Policy1>& lhs,
    const n_array_expression<2, RhsStructure, Evaluator2, Policy2>& rhs) {
  execute(map(f, lhs, rhs));
}
} // namespace satyr
