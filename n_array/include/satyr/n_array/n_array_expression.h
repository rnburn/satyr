#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/structure.h>
#include <satyr/k_array.h>

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
} // namespace satyr
