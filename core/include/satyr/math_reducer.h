#pragma once

#include <satyr/concept.h>
#include <satyr/traits.h>
#include <satyr/index_reducer.h>
#include <satyr/execution_policy.h>
#include <satyr/pragma.h>

namespace satyr {
//------------------------------------------------------------------------------
// sum_reducer
//------------------------------------------------------------------------------
template <IndexFunctor<1> F,
          Scalar T = index_functor_codomain_value_type_t<F, 1>,
          Policy Policy = no_policy>
  requires Scalar<index_functor_codomain_value_type_t<F, 1>>
class sum_reducer {
 public:
   sum_reducer(F f, Policy policy) :
      f_{f} {} 

   sum_reducer(sum_reducer& other, split_reduction)
     : f_{other.f_} {}

   T operator()(index_t first, index_t last) 
     requires !has_policy_v<simd, Policy>
   {
     for (index_t i = first; i != last; ++i) sum_ += f_(i);
   }

   T operator()(index_t first, index_t last) 
     requires has_policy_v<simd, Policy>
   {
     index_t i;
     SATYR_PRAGMA_SIMD_LINEAR_REDUCTION(i, +, sum_)
     for (i = first; i != last; ++i) sum_ += f_(i);
   }

   void join(const sum_reducer& other) { sum_ += other.sum_; }

  private:
   F f_;
   T sum_ = 0;
};
} // namespace satyr
