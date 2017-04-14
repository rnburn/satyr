#pragma once

#include <cstddef>
#include <type_traits>
#include <satyr/index.h>
#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// FlatEvaluator
//------------------------------------------------------------------------------
template <class T>
concept bool FlatEvaluator = 
  requires (T f, index_t i) {
    { f(i) } -> Scalar;
  } && std::is_copy_constructible_v<T>;

//------------------------------------------------------------------------------
// KEvaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class T>
constexpr bool is_k_evaluator_impl = true;

template <size_t... Indexes, class T>
  requires requires (T f, 
                     std::enable_if_t<(Indexes,true), index_t>... indexes) {
    { f(indexes...) } -> Scalar;
  }
constexpr bool is_k_evaluator_impl<std::index_sequence<Indexes...>, T> = true;
}

template <class T, size_t K>
concept bool KEvaluator =
    detail::is_k_evaluator_impl<std::make_index_sequence<2*K>, T>;

//------------------------------------------------------------------------------
// Evaluator
//------------------------------------------------------------------------------
template <class T, size_t K>
concept bool Evaluator = FlatEvaluator<T> || KEvaluator<T, K>;
}  // namespace satyr
