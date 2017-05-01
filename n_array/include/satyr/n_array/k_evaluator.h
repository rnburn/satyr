#pragma once

#include <satyr/n_array/concept.h>
#include <satyr/k_array.h>

namespace satyr {
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
       const satyr::shape<sizeof...(Indexes)>& shape,
       std::enable_if_t<(Indexes, true), index_t>... indexes) const {
     return evaluator_(get_1d_index(shape, indexes...));
   }
 private:
   Evaluator evaluator_;
};
}

template <size_t K, FlatEvaluator Evaluator>
class k_evaluator
    : public detail::k_evaluator_impl<std::make_index_sequence<K>,
                                      Evaluator> {
  using base = detail::k_evaluator_impl<std::make_index_sequence<K>, Evaluator>;

 public:
  explicit k_evaluator(const Evaluator& evaluator)
      : base{evaluator} {}

  using base::base;
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
