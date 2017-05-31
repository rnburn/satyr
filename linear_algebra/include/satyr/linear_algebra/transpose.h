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

  const Evaluator& evaluator() const { return evaluator_; }
 private:
  Evaluator evaluator_;
};

//------------------------------------------------------------------------------
// transpose
//------------------------------------------------------------------------------
inline subshape<2> transpose(const subshape<2>& shape) {
  std::array<index_t, 2> extents = {get_extent<1>(shape), get_extent<0>(shape)};
  std::array<index_t, 2> strides = {get_stride<1>(shape), get_stride<0>(shape)};
  return {satyr::shape<2>{extents}, strides};
}

template <GeneralMatrix Matrix>
auto transpose(Matrix&& matrix) {
  using T = value_type_t<Matrix>;
  if constexpr (detail::match_n_array_subview<uncvref_t<Matrix>>) {
      auto evaluator = n_array_subview_evaluator<T, 2>(
          matrix.data(), matrix.shape().strides());
      return make_n_array_expression<structure_t<Matrix>>(
          transpose(matrix.shape()), evaluator);
  } else {
    auto evaluator = n_array_evaluator<T, 2>(matrix.data());
    return make_n_array_expression<structure_t<Matrix>>(
        transpose(matrix.shape()), evaluator);
  }
}
} // namespace satyr
