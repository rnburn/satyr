#pragma once

#include <tuple>
#include <satyr/index.h>
#include <satyr/n_array/concept.h>
#include <satyr/n_array/k_evaluator.h>

namespace satyr::detail {
//------------------------------------------------------------------------------
// map_evaluator
//------------------------------------------------------------------------------
template <class, class, class Functor, class... Evaluators>
class map_evaluator_impl;

template <size_t... Indexes1, size_t... Indexes2, class Functor,
          class... Evaluators>
class map_evaluator_impl<std::index_sequence<Indexes1...>,
                         std::index_sequence<Indexes2...>, Functor,
                         Evaluators...> {
 public:
  map_evaluator_impl(Functor functor, const Evaluators&... evaluators)
      : functor_{functor}, evaluators_{evaluators...} {}

  decltype(auto) operator()(index_t index) const 
      requires (FlatEvaluator<Evaluators> && ...)
  {
    return functor_(std::get<Indexes1>(evaluators_)(index)...);
  }

  decltype(auto) operator()(
      std::enable_if_t<(Indexes2, true), index_t>... indexes) const 
      requires (KEvaluator<Evaluators, sizeof...(Indexes1)> && ...)
  {
    return functor_(std::get<Indexes1>(evaluators_)(indexes...)...);
  }

 private:
  Functor functor_;
  std::tuple<Evaluators...> evaluators_;
};

template <size_t K, class Functor, class... Evaluators>
    requires (FlatEvaluator<Evaluators> && ...) || 
             (KEvaluator<Evaluators, K> && ...)
class map_evaluator
    : public map_evaluator_impl<std::make_index_sequence<K>,
                                std::make_index_sequence<2 * K>,
                                Functor, Evaluators...> {
 public:
  using map_evaluator_impl<std::make_index_sequence<K>,
                           std::make_index_sequence<2 * K>, Functor,
                           Evaluators...>::map_evaluator_impl;
};

template <size_t K, class Functor, class... Evaluators>
    requires (Evaluator<Evaluators> && ...) &&
             ((FlatEvaluator<Evaluators> && ...) || 
              (KEvaluator<Evaluators, K> && ...))
auto make_map_evaluator(Functor functor, const Evaluators&... evaluators) {
  return map_evaluator<K, Functor, Evaluators...>{functor, evaluators...};
}

template <size_t K, class Functor, class... Evaluators>
    requires (Evaluator<Evaluators> && ...)
auto make_map_evaluator(Functor functor, const Evaluators&... evaluators) {
  return make_map_evaluator<K>(functor, make_k_evaluator<K>(evaluators)...);
}
} // namespace satyr::detail
