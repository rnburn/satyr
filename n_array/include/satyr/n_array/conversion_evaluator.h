#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/scalar_evaluator.h>

namespace satyr {
//------------------------------------------------------------------------------
// make_conversion_evaluator
//------------------------------------------------------------------------------
template <size_t K, Structure FromStructure, Structure ToStructure,
         Evaluator<K> Evaluator>
  requires std::is_same_v<ToStructure, FromStructure>
auto make_conversion_evaluator(FromStructure, ToStructure,
    const Evaluator& evaluator) {
  return evaluator;
}

//------------------------------------------------------------------------------
// convert_evaluator
//------------------------------------------------------------------------------
template <Structure ToStructure, size_t K, Structure FromStructure,
          Evaluator<K> Evaluator>
    requires requires (FromStructure from_structure, ToStructure to_structure,
        const n_array_expression<K, FromStructure, Evaluator>& expression) {
      /* { */ 
        make_conversion_evaluator<K>(from_structure, to_structure, 
                                     expression.evaluator()) ;
      /* } -> satyr::Evaluator<K>; */
    }
auto convert_evaluator(
    const n_array_expression<K, FromStructure, Evaluator>& expression) {
  return make_conversion_evaluator<K>(FromStructure{}, ToStructure{},
                                      expression.evaluator());
}

template <Structure ToStructure, Scalar T>
scalar_evaluator<T> convert_evaluator(T value) {
  return scalar_evaluator(value);
}
} // namespace satyr
