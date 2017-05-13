#pragma once

#include <satyr/n_array/map.h>
#pragma once

#include <satyr/n_array/execute.h>

namespace satyr {
//------------------------------------------------------------------------------
// apply
//------------------------------------------------------------------------------
template <class Functor, size_t K, Structure Structure, Evaluator<K> Evaluator1,
          Policy Policy1, Evaluator<K> Evaluator2, Policy Policy2>
  requires detail::is_mappable_v<
                Functor,
                n_array_expression<K, Structure, Evaluator1, Policy1>,
                n_array_expression<K, Structure, Evaluator2, Policy2>>
void apply(Functor f,
           const n_array_expression<K, Structure, Evaluator1, Policy1>& lhs,
           const n_array_expression<K, Structure, Evaluator2, Policy2>& rhs) {
  execute(map(f, lhs, rhs));
}

template <class Functor, size_t K, Structure Structure, Evaluator<K> Evaluator,
          Policy Policy1, Scalar Scalar, Policy Policy2>
  requires detail::is_mappable_v<
                Functor,
                n_array_expression<K, Structure, Evaluator, Policy1>,
                scalar_expression<Scalar, Policy2>>
void apply(Functor f,
           const n_array_expression<K, Structure, Evaluator, Policy1>& lhs,
           const scalar_expression<Scalar, Policy2>& rhs) {
  execute(map(f, lhs, rhs));
}

//------------------------------------------------------------------------------
// is_applicable_v
//------------------------------------------------------------------------------
template <class Functor, class Lhs, class Rhs>
constexpr bool is_applicable_v = false;

template <class Functor, class Lhs, class Rhs>
  requires requires(Functor f, const Lhs& lhs, const Rhs& rhs) {
    apply(f, lhs, make_expression(rhs));
  }
constexpr bool is_applicable_v<Functor, Lhs, Rhs> = true;
} // namespace satyr
