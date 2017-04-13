#pragma once

#include <tuple>
#include <satyr/index.h>

namespace satyr {
//------------------------------------------------------------------------------
// map_evaluator
//------------------------------------------------------------------------------
template <size_t K, class Functor, class... Evaluators>
class map_evaluator {
 public:
   map_evaluator(Functor functor, Evaluators... evaluators)
     : functor_{functor}, evaluators_{evaluators...}
   {}

   decltype(auto) operator()(index_t index) const {
   }
 private:
   Functor functor_;
   std::tuple<Evaluators...> evaluators_;
};
} // namespace satyr
