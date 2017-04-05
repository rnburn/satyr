#pragma once

#include <satyr/serial_for.h>
#include <satyr/parallel_for.h>

namespace satyr {
template <Policy Policy, size_t K, class Functor>
  requires !has_policy_v<grainularity, Policy>
void for_each_index(Policy policy, std::array<index_t, K> extents,
    Functor f) {
  if constexpr(K == 1) { for_(policy, 0, extents[0], f); }
  else {
    for_(serial_v, 0, extents[K - 1], [=](index_t i) {
      auto f_prime = [=](auto... indexes) { f(indexes..., i); };
      return for_each_index(
          policy, reinterpret_cast<const std::array<index_t, K - 1>&>(extents),
          f_prime);
    });
  }
}

template <Policy Policy, size_t K, class Functor>
  requires has_policy_v<grainularity, Policy>
void for_each_index(Policy policy, std::array<index_t, K> extents,
    Functor f) {
  auto grainularity = get_policy<satyr::grainularity>(policy);
  if (grainularity.value == 1)
    return for_each_index(
        policy | serial_v, extents, f
    );
  if constexpr(K == 1) { for_(policy, 0, extents[0], f); }
  else {
    auto n = extents[K - 1];
    auto grainularity_new =
        satyr::grainularity{std::max<index_t>(grainularity.value / n, 1)};
    for_(grainularity, 0, n, [=](index_t i) {
      auto f_prime = [=](auto... indexes) { f(indexes..., i); };
      return for_each_index(
          policy | grainularity_new,
          reinterpret_cast<const std::array<index_t, K - 1>&>(extents),
          f_prime);
    });
  }
}
} // namespace satyr
