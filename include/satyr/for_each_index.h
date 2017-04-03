#pragma once

#include <satyr/serial_for.h>
#include <satyr/parallel_for.h>

namespace satyr {
template <Policy Policy, size_t K, class Functor>
  requires !has_policy_v<grainularity, Policy>
void for_each_index(Policy policy, std::array<index_t, K> extents,
    Functor f) {
  if constexpr(K == 1) { 
    for_(policy, 0, extents[0], f); 
  } else {
    for (index_t i = 0; i < extents[K - 1]; ++i) {
      auto f_prime = [=](auto... indexes) { f(indexes..., i); };
      for_each_index(policy,
                     reinterpret_cast<std::array<index_t, K - 1>&>(extents),
                     f_prime);
    }
  }
}
} // namespace satyr
