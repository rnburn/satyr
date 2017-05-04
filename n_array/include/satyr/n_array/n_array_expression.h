#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/structure.h>
#include <satyr/k_array.h>
#include <satyr/execution_policy.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_expression
//------------------------------------------------------------------------------
template <size_t K, Structure Structure, Evaluator<K> Evaluator>
class n_array_expression {
 public:
   using structure = Structure;

   n_array_expression(const satyr::shape<K>& shape, const Evaluator& evaluator)
     : shape_{shape}, evaluator_{evaluator} {}

   const satyr::shape<K>& shape() const { return shape_; }

   const Evaluator& evaluator() const { return evaluator_; }
 private:
   satyr::shape<K> shape_;
   Evaluator evaluator_;
};

template <size_t K, Structure Structure, Evaluator<K> Evaluator, 
         Policy Policy>
class n_array_expression2 {
 public:
   using structure = Structure;

   n_array_expression2(const satyr::shape<K>& shape, const Evaluator& evaluator,
                      Policy policy)
       : shape_{shape},
         evaluator_{evaluator},
         policy_{policy} {}

   const satyr::shape<K>& shape() const { return shape_; }

   const Evaluator& evaluator() const { return evaluator_; }

   Policy policy() const { return policy_; }

 private:
   satyr::shape<K> shape_;
   Evaluator evaluator_;
   Policy policy_;
};

//------------------------------------------------------------------------------
// make_n_array_expression
//------------------------------------------------------------------------------
template <Structure Structure, size_t K, Evaluator<K> Evaluator>
n_array_expression<K, Structure, Evaluator> make_n_array_expression(
    const shape<K>& shape, const Evaluator& evaluator) {
  return {shape, evaluator};
}

template <Structure Structure, size_t K, Evaluator<K> Evaluator, Policy Policy>
n_array_expression2<K, Structure, Evaluator, Policy> make_n_array_expression2(
    const shape<K>& shape, const Evaluator& evaluator, Policy policy) {
  return {shape, evaluator, policy};
}

template <Structure Structure, size_t K, Evaluator<K> Evaluator>
auto make_n_array_expression2(const shape<K>& shape,
                              const Evaluator& evaluator) {
  return make_n_array_expression2<Structure>(shape, evaluator, no_policy_v);
}
} // namespace satyr
