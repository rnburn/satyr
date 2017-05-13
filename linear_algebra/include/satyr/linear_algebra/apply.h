#pragma once

#include <satyr/linear_algebra/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// apply
//------------------------------------------------------------------------------
template <class Functor, Evaluator<2> Evaluator1, Policy Policy1,
          KEvaluator<2> Evaluator2, Policy Policy2>
  requires detail::is_mappable_v<
                Functor,
                n_array_expression<2, general_structure, Evaluator1, Policy1>,
                n_array_expression<2, symmetric_structure, Evaluator2, Policy2>>
void apply(
    Functor f,
    const n_array_expression<2, general_structure, Evaluator1, Policy1>& lhs,
    const n_array_expression<2, symmetric_structure, Evaluator2, Policy2>&
        rhs) {
  execute(map(f, lhs, rhs));
}

template <class Functor, Evaluator<2> Evaluator1, Policy Policy1,
          Structure RhsStructure, KEvaluator<2> Evaluator2, Policy Policy2>
  requires (std::is_same_v<RhsStructure, lower_triangular_structure> ||
            std::is_same_v<RhsStructure, upper_triangular_structure>) &&
           detail::is_mappable_v<
                Functor,
                n_array_expression<2, general_structure, Evaluator1, Policy1>,
                n_array_expression<2, RhsStructure, Evaluator2, Policy2>>
void apply(
    Functor f,
    const n_array_expression<2, general_structure, Evaluator1, Policy1>& lhs,
    const n_array_expression<2, RhsStructure, Evaluator2, Policy2>& rhs) {
  auto lhs_new = make_n_array_expression<RhsStructure>(
      lhs.shape(), make_k_evaluator<2>(lhs.evaluator()), lhs.policy());
  execute(map(f, lhs_new, rhs));
}
} // namespace satyr
