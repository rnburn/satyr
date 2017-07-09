#pragma once

#include <satyr/concept.h>
#include <satyr/traits.h>
#include <satyr/index_reducer.h>
#include <satyr/execution_policy.h>
#include <satyr/pragma.h>
#include <algorithm>
#include <limits>

namespace satyr {
//------------------------------------------------------------------------------
// sum_reducer
//------------------------------------------------------------------------------
template <Scalar T>
class sum_reducer {
 public:
   using value_type = T;

   sum_reducer() = default;

   sum_reducer(sum_reducer& other, split_reduction) {}

   template <Policy Policy, IndexFunctor<1> F>
     requires !has_policy_v<simd, Policy> &&
              Scalar<index_functor_codomain_value_type_t<F, 1>>
   T operator()(Policy /*policy*/, index_t first, index_t last, F f) {
     for (index_t i = first; i != last; ++i) value_ += f(i);
   }

   template <Policy Policy, IndexFunctor<1> F>
     requires has_policy_v<simd, Policy> &&
              Scalar<index_functor_codomain_value_type_t<F, 1>>
   T operator()(Policy /*policy*/, index_t first, index_t last, F f) {
     index_t i;
     SATYR_PRAGMA_SIMD_LINEAR_REDUCTION(i, +, value_)
     for (i = first; i != last; ++i) value_ += f(i);
   }

   void join(const sum_reducer& other) { value_ += other.value_; }

  private:
   T value_ = 0;
};

//------------------------------------------------------------------------------
// max_reducer
//------------------------------------------------------------------------------
template <RealScalar T>
class max_reducer {
  public:
   using value_type = T;

   max_reducer() = default;

   max_reducer(max_reducer& other, split_reduction) {}

   template <Policy Policy, IndexFunctor<1> F>
     requires !has_policy_v<simd, Policy> &&
              std::is_same_v<index_functor_codomain_value_type_t<F, 1>, T>
   T operator()(Policy /*policy*/, index_t first, index_t last, F f) {
     for (index_t i = first; i != last; ++i) value_ = std::max(value_, f(i));
   }

   template <Policy Policy, IndexFunctor<1> F>
     requires has_policy_v<simd, Policy> &&
              std::is_same_v<index_functor_codomain_value_type_t<F, 1>, T>
   T operator()(Policy /*policy*/, index_t first, index_t last, F f) {
     index_t i;
     SATYR_PRAGMA_SIMD_LINEAR_REDUCTION(i, max, value_)
     for (i = first; i != last; ++i) value_ = std::max(value_, f(i));
   }

   void join(const max_reducer& other) {
     value_ = std::max(other.value_, value_);
   }

  private:
   T value_ = std::numeric_limits<T>::lowest();
};

//------------------------------------------------------------------------------
// min_reducer
//------------------------------------------------------------------------------
template <RealScalar T>
class min_reducer {
  public:
   using value_type = T;

   min_reducer() = default;

   min_reducer(min_reducer& other, split_reduction) {}

   template <Policy Policy, IndexFunctor<1> F>
     requires !has_policy_v<simd, Policy> &&
              std::is_same_v<index_functor_codomain_value_type_t<F, 1>, T>
   T operator()(Policy /*policy*/, index_t first, index_t last, F f) {
     for (index_t i = first; i != last; ++i) value_ = std::min(value_, f(i));
   }

   template <Policy Policy, IndexFunctor<1> F>
     requires has_policy_v<simd, Policy> &&
              std::is_same_v<index_functor_codomain_value_type_t<F, 1>, T>
   T operator()(Policy /*policy*/, index_t first, index_t last, F f) {
     index_t i;
     SATYR_PRAGMA_SIMD_LINEAR_REDUCTION(i, min, value_)
     for (i = first; i != last; ++i) value_ = std::min(value_, f(i));
   }

   void join(const min_reducer& other) {
     value_ = std::min(other.value_, value_);
   }

  private:
   T value_ = std::numeric_limits<T>::max();
};
} // namespace satyr
