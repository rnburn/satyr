#pragma once

#include <satyr/n_array.h>
#include <satyr/linear_algebra/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// diagonal_matrix_evaluator
//------------------------------------------------------------------------------
template <KEvaluator<1> Evaluator>
class diagonal_matrix_evaluator {
 public:
  explicit diagonal_matrix_evaluator(const Evaluator& evaluator)
      : evaluator_{evaluator} {}

  auto operator()(const shape<2>& shape, index_t i, index_t j) const {
    return (i == j) * evaluator_({get_extent<0>(shape)}, i);
  }

 private:
  Evaluator evaluator_;
};

//------------------------------------------------------------------------------
// make_diagonal_matrix_evaluator
//------------------------------------------------------------------------------
template <Evaluator<1> Evaluator>
auto make_diagonal_matrix_evaluator(const Evaluator& evaluator) {
  return diagonal_matrix_evaluator(make_k_evaluator<1>(evaluator));
}

//------------------------------------------------------------------------------
// as_diagonal_matrix
//------------------------------------------------------------------------------
template <NArrayExpressible Expressible>
  requires num_dimensions_v<Expressible> == 1 &&
           std::is_same_v<structure_t<Expressible>, general_structure>
auto as_diagonal_matrix(const Expressible& expressible) {
  auto expression = make_expression(expressible);
  return make_n_array_expression<diagonal_structure>(
      satyr::shape(get_extent<0>(expressible), get_extent<0>(expressible)),
      make_diagonal_matrix_evaluator(expression.evaluator()));
}

//------------------------------------------------------------------------------
// identity_matrix
//------------------------------------------------------------------------------
inline auto identity_matrix(index_t n) {
  return map_indexes<diagonal_structure>(
      satyr::shape(n, n), [](index_t i, index_t j) -> int { return i == j; });
}
} // namespace satyr
