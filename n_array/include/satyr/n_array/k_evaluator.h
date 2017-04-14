#pragma once

#include <satyr/n_array/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// flatten_indexes
//------------------------------------------------------------------------------
namespace detail {
template <class... IndexesRest>
index_t flatten_indexes(index_t index, index_t size,
                        IndexesRest... indexes_rest) {
  if constexpr(sizeof...(IndexesRest)) 
    return index + size * flatten_indexes(indexes_rest...);
  else
    return index;
}
}

//------------------------------------------------------------------------------
// k_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class Evaluator>
class k_evaluator_impl;

template <size_t... Indexes, class Evaluator>
class k_evaluator_impl<std::index_sequence<Indexes...>, Evaluator> {
 public:
   explicit k_evaluator_impl(const Evaluator& evaluator)
     : evaluator_{evaluator}
   {}

   decltype(auto) operator()(
       std::enable_if_t<(Indexes, true), index_t>... indexes) const {
     return evaluator_(flatten_indexes(indexes...));
   }
 private:
   Evaluator evaluator_;
};
}

template <size_t K, FlatEvaluator Evaluator>
class k_evaluator
    : public detail::k_evaluator_impl<std::make_index_sequence<2 * K>,
                                      Evaluator> {
 public:
  using detail::k_evaluator_impl<std::make_index_sequence<2 * K>,
                                 Evaluator>::k_evaluator_impl;
};

//------------------------------------------------------------------------------
// make_k_evaluator
//------------------------------------------------------------------------------
template <size_t K, FlatEvaluator Evaluator>
auto make_k_evaluator(const Evaluator& evaluator) {
  return k_evaluator<K, Evaluator>{evaluator};
}

template <size_t K, KEvaluator<K> Evaluator>
auto make_k_evaluator(const Evaluator& evaluator) {
  return evaluator;
}
} // namespace satyr
