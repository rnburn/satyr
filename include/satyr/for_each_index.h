#pragma once

#include <satyr/serial_for.h>
#include <satyr/parallel_for.h>
#include <satyr/uplo.h>

namespace satyr {
//------------------------------------------------------------------------------
// for_each_index
//------------------------------------------------------------------------------
template <Policy Policy, size_t K, class Functor>
  requires !has_policy_v<grainularity, Policy>
       && (IndexFunctor<Functor, void, K> || IndexFunctor<Functor, bool, K>)
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

namespace detail {
template <class Policy, size_t K, class Functor>
void parallel_for_each_index_impl(Policy policy,
                                  std::array<index_t, K-1> cardinalities,
                                  std::array<index_t, K> extents,
                                  Functor f) {
  if constexpr(K == 1) { 
    for_(policy, 0, extents[0], f); 
  } else {
    auto n = cardinalities[K - 2];
    auto grainularity = get_policy<satyr::grainularity>(policy);
    auto grainularity_new =
        satyr::grainularity{std::max<index_t>(grainularity.value / n, 1)};
    for_(grainularity_new, 0, n, [=](index_t i) {
      auto f_prime = [=](auto... indexes) { f(indexes..., i); };
      return parallel_for_each_index_impl(
          policy,
          reinterpret_cast<const std::array<index_t, K - 2>&>(extents),
          reinterpret_cast<const std::array<index_t, K - 1>&>(extents),
          f_prime);
    });
  }
}
}

template <Policy Policy, size_t K, class Functor>
  requires has_policy_v<grainularity, Policy> 
       && (IndexFunctor<Functor, void, K> || IndexFunctor<Functor, bool, K>)
void for_each_index(Policy policy, std::array<index_t, K> extents, Functor f) {
  std::array<index_t, K - 1> cardinalities;
  index_t cardinality = 1;
  for (index_t i = 0; i < K - 1; ++i) {
    cardinality *= extents[i];
    cardinalities[i] = cardinality;
  }
  detail::parallel_for_each_index_impl(policy, cardinalities, extents, f);
}

//------------------------------------------------------------------------------
// for_each_index_half
//------------------------------------------------------------------------------
template <uplo_t Uplo, Policy Policy, class Functor>
  requires std::is_same_v<Uplo, uplo_t::lower>
       && !has_policy_v<grainularity, Policy>
       && (IndexFunctor<Functor, void, 2> || IndexFunctor<Functor, bool, 2>)
auto for_each_index_half(Policy policy, std::array<index_t, 2> extents,
                         Functor f) {
  return for_(no_policy_v, 0, extents[1], [=](index_t j) {
    return for_(policy, j, extents[0], [=](index_t i) { return f(i, j); });
  });
}

template <uplo_t Uplo, Policy Policy, class Functor>
  requires std::is_same_v<Uplo, uplo_t::upper>
       && !has_policy_v<grainularity, Policy>
       && (IndexFunctor<Functor, void, 2> || IndexFunctor<Functor, bool, 2>)
auto for_each_index_half(Policy policy, std::array<index_t, 2> extents,
                         Functor f) {
  return for_(no_policy_v, 0, extents[1], [=](index_t j) {
    return for_(policy, 0, j+1, [=](index_t i) { return f(i, j); });
  });
}
} // namespace satyr
