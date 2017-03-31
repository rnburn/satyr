#pragma once

#include <satyr/execution_policy.h>
#include <satyr/concept.h>
#include <satyr/pragma.h>

namespace satyr {
template <Policy Policy, Functor<void(index_t)> F>
  requires !has_policy_v<grainularity, Policy>
void for_(Policy policy, index_t i, index_t last, F f) {
  for(; i<last; ++i) f(i);
}

template <Policy Policy, Functor<void(index_t)> F>
  requires !has_policy_v<grainularity, Policy> &&
           has_policy_v<simd, Policy>
void for_(Policy policy, index_t first, index_t last, F f) {
  index_t i;
  SATYR_PRAGMA_SIMD_LINEAR(i)
  for(i=first; i<last; ++i) f(i);
}

template <Policy Policy, Functor<bool(index_t)> F>
  requires !has_policy_v<grainularity, Policy>
void for_(Policy policy, index_t i, index_t last, F f) {
  for (; i < last; ++i)
    if (!f(i)) return;
}

template <Policy Policy, Functor<bool(index_t)> F>
  requires !has_policy_v<grainularity, Policy> &&
           has_policy_v<simd, Policy>
void for_(Policy policy, index_t first, index_t last, F f) {
  index_t i;
  SATYR_PRAGMA_SIMD_LINEAR(i)
  for (i = first; i < last; ++i)
    if (!f(i)) return;
}
} // namespace satyr
