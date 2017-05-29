#pragma once

#include <satyr/linear_algebra/concept.h>
#include <satyr/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// transpose_evaluator
//------------------------------------------------------------------------------
template <KEvaluator<2> Evaluator>
class transpose_evaluator {
 public:
  explicit transpose_evaluator(const Evaluator& evaluator)
      : evaluator_{evaluator} {}

  decltype(auto) operator()(const shape<2>& shape, index_t i, index_t j) const {
    return evaluator_(
        satyr::shape<2>{get_extent<1>(shape), get_extent<0>(shape)}, j, i);
  }

 private:
  Evaluator evaluator_;
};

//------------------------------------------------------------------------------
// transpose
//------------------------------------------------------------------------------
template <Matrix Matrix>
  requires std::is_same_v<structure_t<Matrix>, general_structure>
auto transpose(Matrix&& matrix) {
  auto expression = make_expression(matrix);
  auto shape = satyr::shape<2>{get_extent<1>(matrix), get_extent<0>(matrix)};
  return make_n_array_expression<structure_t<Matrix>>(
      shape, transpose_evaluator(expression.evaluator()));
}
} // namespace satyr
