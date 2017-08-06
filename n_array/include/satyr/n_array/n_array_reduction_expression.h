#pragma once

#include <satyr/execution_policy.h>
#include <satyr/index_reducer.h>
#include <satyr/k_array.h>
#include <satyr/n_array/concept.h>
#include <satyr/n_array/structure.h>
#include <satyr/traits.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_reduction_expression
//------------------------------------------------------------------------------
template <size_t K, Structure Structure, IndexReducer Reducer,
          Evaluator<K> Evaluator, Policy Policy>
  requires std::is_convertible_v<codomain_t<Evaluator>, value_type_t<Reducer>> 
class n_array_reduction_expression {
 public:
  using structure = Structure;
  using value_type = value_type_t<Reducer>;
  using reducer_type = Reducer;

  n_array_reduction_expression(const satyr::shape<K>& shape,
                               const Evaluator& evaluator, Policy policy)
      : shape_{shape}, evaluator_{evaluator}, policy_{policy} {}

  const satyr::shape<K>& shape() const { return shape_; }

  const Evaluator& evaluator() const { return evaluator_; }

  Policy policy() const { return policy_; }

 private:
  satyr::shape<K> shape_;
  Evaluator evaluator_;
  Policy policy_;
};

//------------------------------------------------------------------------------
// make_n_array_reduction_expression
//------------------------------------------------------------------------------
template <Structure Structure, IndexReducer Reducer, size_t K,
          Evaluator<K> Evaluator, Policy Policy>
  requires std::is_convertible_v<codomain_t<Evaluator>, value_type_t<Reducer>> 
n_array_reduction_expression<K, Structure, Reducer, Evaluator, Policy>
make_n_array_reduction_expression(const shape<K>& shape,
                                  const Evaluator& evaluator, Policy policy) {
  return {shape, evaluator, policy};
}

template <Structure Structure, IndexReducer Reducer, size_t K,
          Evaluator<K> Evaluator, Policy Policy>
  requires std::is_convertible_v<codomain_t<Evaluator>, value_type_t<Reducer>> 
auto make_n_array_reduction_expression(const shape<K>& shape,
                                       const Evaluator& evaluator) {
  return make_n_array_reduction_expression(shape, evaluator, no_policy_v);
}

template <Structure Structure, IndexReducer Reducer, size_t K,
          Evaluator<K> Evaluator, Policy Policy>
  requires std::is_convertible_v<codomain_t<Evaluator>, value_type_t<Reducer>> 
n_array_reduction_expression<K, Structure, Reducer, Evaluator, Policy>
make_n_array_reduction_expression(const subshape<K>& shape,
                                  const Evaluator& evaluator, Policy policy) {
  return {shape, evaluator, policy};
}

template <Structure Structure, IndexReducer Reducer, size_t K,
          Evaluator<K> Evaluator, Policy Policy>
  requires std::is_convertible_v<codomain_t<Evaluator>, value_type_t<Reducer>> 
auto make_n_array_reduction_expression(const subshape<K>& shape,
                                       const Evaluator& evaluator) {
  return make_n_array_reduction_expression(shape, evaluator, no_policy_v);
}
}  // namespace satyr
