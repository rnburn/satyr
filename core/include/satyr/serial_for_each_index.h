#pragma once

#include <satyr/serial_for.h>
#include <satyr/blocked_range.h>
#include <satyr/matrix.h>

namespace satyr {
//------------------------------------------------------------------------------
// for_each_index
//------------------------------------------------------------------------------
// k-cell
template <Policy Policy, size_t K, IndexFunctor<K> Functor>
  requires !has_policy_v<grainsize, Policy>
void for_each_index(Policy policy, std::array<index_t, K> extents,
    Functor f) {
  if constexpr(K == 1) { for_(policy, 0, extents[0], f); }
  else {
    for_(serial_v, 0, extents[K - 1], [=](index_t i) {
      auto f_prime = [=](auto... indexes) { f(indexes..., i); };
      for_each_index(
          policy, reinterpret_cast<const std::array<index_t, K - 1>&>(extents),
          f_prime);
    });
  }
}

// k-cell blocked_range
namespace detail {
template <size_t I, Policy Policy, size_t K, class Functor>
void blocked_range_for_each_index_impl(Policy policy,
                                       const k_blocked_range<K>& range,
                                       Functor f) {
  if
    constexpr(I == 0) {
      for_(policy, range.template first<0>(), range.template last<0>(), f);
    }
  else {
    for_(serial_v, range.template first<I>(), range.template last<I>(),
         [policy, &range, f](index_t i) {
           auto f_prime = [=](auto... indexes) { f(indexes..., i); };
           blocked_range_for_each_index_impl<I - 1>(policy, range, f_prime);
         });
  }
}
} // namespace detail

template <Policy Policy, size_t K, IndexFunctor<K> Functor>
  requires !has_policy_v<grainsize, Policy>
void for_each_index(Policy policy, const k_blocked_range<K>& range,
    Functor f) {
  detail::blocked_range_for_each_index_impl<K-1>(policy, range, f);
}

///------------------------------------------------------------------------------/
// for_each_index_triangular
//------------------------------------------------------------------------------
namespace detail {
template <uplo_t Uplo, class Policy, class Functor>
    requires Uplo == uplo_t::lower 
void for_each_index_triangular_impl(Policy policy, index_t j, index_t n,
                                    Functor f) {
  for_(policy, j, n, [=](index_t i) { f(i, j); });
}

template <uplo_t Uplo, class Policy, class Functor>
    requires Uplo == uplo_t::upper 
void for_each_index_triangular_impl(Policy policy, index_t j, index_t n,
                                    Functor f) {
  for_(policy, 0, j+1, [=](index_t i) { f(i, j); });
}
}  // namespace detail

// n
template <uplo_t Uplo, Policy Policy, IndexFunctor<2> Functor>
  requires !has_policy_v<grainsize, Policy>
void for_each_index_triangular(Policy policy, index_t n, Functor f) {
  for_(no_policy_v, 0, n, [=](index_t j) {
    detail::for_each_index_triangular_impl<Uplo>(policy, j, n, f);
  });
}

// triangular blocked-range
template <uplo_t Uplo, Policy Policy, IndexFunctor<2> Functor>
  requires !has_policy_v<grainsize, Policy>
void for_each_index_triangular(Policy policy,
                               const triangular_blocked_range<Uplo>& range,
                               Functor f) {
  if (range.is_column_range()) {
    auto [j_first, j_last] = range.columns();
    auto n = range.n();
    for_(no_policy_v, j_first, j_last, [=](index_t j) {
        detail::for_each_index_triangular_impl<Uplo>(policy, j, n, f);
    });
  } else {
    auto [j, i_first, i_last] = range.column_rows();
    for_(policy, i_first, i_last, [=](index_t i) { f(i, j); });
  }
}

//------------------------------------------------------------------------------
// for_each_index_with_exit
//------------------------------------------------------------------------------
// k-cell
template <Policy Policy, size_t K, IndexPredicate<K> Predicate>
  requires !has_policy_v<grainsize, Policy>
bool for_each_index_with_exit(Policy policy, std::array<index_t, K> extents,
    Predicate f) {
  if constexpr(K == 1) { 
    return for_with_exit(policy, 0, extents[0], f); 
  } else {
    return for_with_exit(serial_v, 0, extents[K - 1], [=](index_t i) {
      auto f_prime = [=](auto... indexes) { return f(indexes..., i); };
      return for_each_index_with_exit(
          policy, reinterpret_cast<const std::array<index_t, K - 1>&>(extents),
          f_prime);
    });
  }
}

// k-cell blocked_range
namespace detail {
template <size_t I, Policy Policy, size_t K, class Predicate>
bool blocked_range_for_each_index_with_exit_impl(Policy policy,
                                                 const k_blocked_range<K>& range,
                                                 Predicate f) {
  if constexpr(I == 0) {
    return for_with_exit(policy, range.template first<0>(),
                           range.template last<0>(), f);
  } else {
    return for_with_exit(
        serial_v, range.template first<I>(), range.template last<I>(),
        [policy, &range, f](index_t i) {
          auto f_prime = [=](auto... indexes) { return f(indexes..., i); };
          return blocked_range_for_each_index_with_exit_impl<I - 1>(
              policy, range, f_prime);
        });
  }
}
} // namespace detail

template <Policy Policy, size_t K, IndexPredicate<K> Predicate>
  requires !has_policy_v<grainsize, Policy>
bool for_each_index_with_exit(Policy policy, const k_blocked_range<K>& range,
    Predicate f) {
  return detail::blocked_range_for_each_index_with_exit_impl<K - 1>(policy,
                                                                    range, f);
}

//------------------------------------------------------------------------------
// for_each_index_triangular_with_exit
//------------------------------------------------------------------------------
namespace detail {
template <uplo_t Uplo, class Policy, class Predicate>
    requires Uplo == uplo_t::lower 
bool for_each_index_triangular_with_exit_impl(Policy policy, index_t j,
                                                index_t n, Predicate f) {
  return for_with_exit(policy, j, n, [=](index_t i) { return f(i, j); });
}

// n
template <uplo_t Uplo, class Policy, class Predicate>
    requires Uplo == uplo_t::upper 
bool for_each_index_triangular_with_exit_impl(Policy policy, index_t j,
                                              index_t n, Predicate f) {
  return for_with_exit(policy, 0, j+1, [=](index_t i) { return f(i, j); });
}
}  // namespace detail

template <uplo_t Uplo, Policy Policy, IndexPredicate<2> Predicate>
  requires !has_policy_v<grainsize, Policy>
bool for_each_index_triangular_with_exit(Policy policy, index_t n,
                                         Predicate f) {
  return for_with_exit(no_policy_v, 0, n, [=](index_t j) {
    return detail::for_each_index_triangular_with_exit_impl<Uplo>(policy, j,
                                                                    n, f);
  });
}

// triangular blocked-range
template <uplo_t Uplo, Policy Policy, IndexPredicate<2> Predicate>
  requires !has_policy_v<grainsize, Policy>
bool for_each_index_triangular_with_exit(
    Policy policy, const triangular_blocked_range<Uplo>& range, Predicate f) {
  if (range.is_column_range()) {
    auto [j_first, j_last] = range.columns();
    auto n = range.n();
    return for_with_exit(no_policy_v, j_first, j_last, [=](index_t j) {
      return detail::for_each_index_triangular_with_exit_impl<Uplo>(policy, j,
                                                                    n, f);
    });
  } else {
    auto [j, i_first, i_last] = range.column_rows();
    return for_with_exit(policy, i_first, i_last,
                         [=](index_t i) { return f(i, j); });
  }
}
} // namesapce satyr
