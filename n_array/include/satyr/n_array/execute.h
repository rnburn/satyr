#pragma once

#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/concept.h>

namespace satyr {
template <size_t K, Structure Structure, FlatEvaluator Evaluator>
  requires std::is_base_of_v<general_structure, Structure>
void execute(const n_array_expression<K, Structure, Evaluator>& expression) {
  
}
} // namespace satyr
