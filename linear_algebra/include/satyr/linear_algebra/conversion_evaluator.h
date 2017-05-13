#pragma once

#include <satyr/n_array.h>
#include <satyr/linear_algebra/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// symmetric_to_general_evaluator
//------------------------------------------------------------------------------
template <KEvaluator<2> Evaluator>
class symmetric_to_general_evaluator {
 public:
  explicit symmetric_to_general_evaluator(const Evaluator& evaluator)
    : evaluator_{evaluator}
  {}

  auto operator()(const satyr::shape<2>& shape, index_t i, index_t j) const {
    return evaluator_(shape, std::max(i, j), std::min(i, j));
  }
 private:
  Evaluator evaluator_; 
};

//------------------------------------------------------------------------------
// lower_triangular_to_general_evaluator
//------------------------------------------------------------------------------
template <KEvaluator<2> Evaluator>
class lower_triangular_to_general_evaluator {
 public:
  explicit lower_triangular_to_general_evaluator(const Evaluator& evaluator)
    : evaluator_{evaluator}
  {}

  auto operator()(const satyr::shape<2>& shape, index_t i, index_t j) const {
    return (i < j) ? 0 : evaluator_(shape, i, j);
  }
 private:
  Evaluator evaluator_; 
};

//------------------------------------------------------------------------------
// upper_triangular_to_general_evaluator
//------------------------------------------------------------------------------
template <KEvaluator<2> Evaluator>
class upper_triangular_to_general_evaluator {
 public:
  explicit upper_triangular_to_general_evaluator(const Evaluator& evaluator)
    : evaluator_{evaluator}
  {}

  auto operator()(const satyr::shape<2>& shape, index_t i, index_t j) const {
    return (i > j) ? 0 : evaluator_(shape, i, j);
  }
 private:
  Evaluator evaluator_; 
};

//------------------------------------------------------------------------------
// convert_evaluator
//------------------------------------------------------------------------------
template <size_t K, Structure FromStructure, Structure ToStructure,
          KEvaluator<K> Evaluator>
    requires K == 2 && std::is_same_v<FromStructure, symmetric_structure> &&
             std::is_same_v<ToStructure, general_structure>
symmetric_to_general_evaluator<Evaluator> convert_evaluator(
    const Evaluator& evaluator) {
  return symmetric_to_general_evaluator(evaluator);
}

template <size_t K, Structure FromStructure, Structure ToStructure,
          KEvaluator<K> Evaluator>
    requires K == 2 && std::is_same_v<FromStructure, lower_triangular_structure> &&
             std::is_same_v<ToStructure, general_structure>
lower_triangular_to_general_evaluator<Evaluator> convert_evaluator(
    const Evaluator& evaluator) {
  return lower_triangular_to_general_evaluator(evaluator);
}

template <size_t K, Structure FromStructure, Structure ToStructure,
          KEvaluator<K> Evaluator>
    requires K == 2 && std::is_same_v<FromStructure, upper_triangular_structure> &&
             std::is_same_v<ToStructure, general_structure>
upper_triangular_to_general_evaluator<Evaluator> convert_evaluator(
    const Evaluator& evaluator) {
  return upper_triangular_to_general_evaluator(evaluator);
}
} // namespace satyr
