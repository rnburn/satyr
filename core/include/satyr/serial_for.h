#pragma once

#include <satyr/execution_policy.h>
#include <satyr/concept.h>
#include <satyr/pragma.h>

namespace satyr {
//------------------------------------------------------------------------------
// for_
//------------------------------------------------------------------------------
template <Policy Policy, IndexFunctor<1> F>
  requires !has_policy_v<grainsize, Policy>
void for_(Policy /*policy*/, index_t i, index_t last, F f) {
  for(; i<last; ++i) f(i);
}

template <Policy Policy, IndexFunctor<1> F>
  requires !has_policy_v<grainsize, Policy> &&
           has_policy_v<simd, Policy>
void for_(Policy /*policy*/, index_t first, index_t last, F f) {
  index_t i;
  SATYR_PRAGMA_SIMD_LINEAR(i)
  for(i=first; i<last; ++i) f(i);
}

//------------------------------------------------------------------------------
// for_with_exit
//------------------------------------------------------------------------------
template <Policy Policy, IndexPredicate<1> F>
  requires !has_policy_v<grainsize, Policy>
bool for_with_exit(Policy /*policy*/, index_t i, index_t last, F f) {
  for(; i<last; ++i)
    if (!f(i)) return false;
  return true;
}

template <Policy Policy, IndexPredicate<1> F>
  requires !has_policy_v<grainsize, Policy> &&
           has_policy_v<simd, Policy>
bool for_with_exit(Policy /*policy*/, index_t first, index_t last, F f) {
  index_t i;
  // Disable vectorization for now; gcc bug
  //    https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81052
  /* SATYR_PRAGMA_SIMD_LINEAR(i) */
  for (i = first; i < last; ++i)
    if (!f(i)) return false;
  return true;
}
} // namespace satyr
