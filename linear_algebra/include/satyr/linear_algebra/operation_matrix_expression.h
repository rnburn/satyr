#pragma once

#include <satyr/linear_algebra/transpose.h>
#include <satyr/matrix.h>

namespace satyr {
//------------------------------------------------------------------------------
// matrix_operation_v
//------------------------------------------------------------------------------
template <OperationMatrix T>
constexpr matrix_operation_t matrix_operation_v = matrix_operation_t::none;

template <OperationMatrix T>
  requires detail::match_transposed_matrix<uncvref_t<T>>
constexpr matrix_operation_t matrix_operation_v<T> =
                  matrix_operation_t::transpose;

//------------------------------------------------------------------------------
// n_array_expression
//------------------------------------------------------------------------------
namespace detail {
template <class T>
constexpr bool match_operation_matrix_evaluator = false;

template <Scalar Scalar>
constexpr bool match_operation_matrix_evaluator<
    transpose_evaluator<n_array_evaluator<Scalar, 2>>> = true;

template <Scalar Scalar>
constexpr bool match_operation_matrix_evaluator<
    transpose_evaluator<n_array_subview_evaluator<Scalar, 2>>> = true;
}  // namespace detail

template <Structure Structure, Evaluator<2> Evaluator, Policy Policy>
  requires detail::match_operation_matrix_evaluator<Evaluator>
class n_array_expression<2, Structure, Evaluator, Policy> {
 public:
   using structure = Structure;
   using value_type = uncvref_t<codomain_t<Evaluator>>;

   n_array_expression(const satyr::subshape<2>& shape, const Evaluator& evaluator,
                      Policy policy)
       : shape_{shape},
         evaluator_{evaluator},
         policy_{policy} {}

   const satyr::subshape<2>& shape() const { return shape_; }

   const Evaluator& evaluator() const { return evaluator_; }

   Policy policy() const { return policy_; }

   auto data() const { return evaluator_.evaluator().data(); }

   decltype(auto) operator()(index_t i, index_t j) const {
     return evaluator_(shape_, i, j);
   }
 private:
   satyr::subshape<2> shape_;
   Evaluator evaluator_;
   Policy policy_;
};
} // namespace satyr
