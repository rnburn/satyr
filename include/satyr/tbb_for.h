#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_for.h>
#include <satyr/matrix.h>
#include <tbb/parallel_for.h>
#include <tbb/task.h>

namespace satyr {
//------------------------------------------------------------------------------
// for_
//------------------------------------------------------------------------------
template <Policy Policy, IndexFunctor<1> F>
  requires has_policy_v<grainularity, Policy>
void for_(Policy policy, index_t first, index_t last, F f) {
  auto grainularity = get_policy<satyr::grainularity>(policy);
  tbb::parallel_for(
      tbb::blocked_range<index_t>{first, last,
                                  static_cast<size_t>(grainularity.value)},
      [=](const tbb::blocked_range<index_t>& range) {
        for_(policy | serial_v, range.begin(), range.end(), f);
      });
}

//------------------------------------------------------------------------------
// for_with_cancel
//------------------------------------------------------------------------------
namespace detail {
template <Policy Policy, IndexPredicate<1> F>
  requires has_policy_v<grainularity, Policy>
bool for_with_cancel_impl(tbb::task_group_context& group, Policy policy,
                          index_t first, index_t last, F f) {
  auto grainularity = get_policy<satyr::grainularity>(policy);
  tbb::parallel_for(
      tbb::blocked_range<index_t>{first, last,
                                  static_cast<size_t>(grainularity.value)},
      [=, &group](const tbb::blocked_range<index_t>& range) {
        if (!for_with_cancel(policy | serial_v, range.begin(), range.end(), f))
          group.cancel_group_execution();
      },
      tbb::auto_partitioner{}, group);
  return !group.is_group_execution_cancelled();
}
} // namespace detail

template <Policy Policy, IndexPredicate<1> F>
  requires has_policy_v<grainularity, Policy>
bool for_with_cancel(Policy policy, index_t first, index_t last, F f) {
  tbb::task_group_context group;
  return detail::for_with_cancel_impl(group, policy, first, last, f);
}

//------------------------------------------------------------------------------
// for_each_index_triangular_with_cancel
//------------------------------------------------------------------------------
namespace detail {
template <uplo_t Uplo, class Policy, class Predicate>
    requires Uplo == uplo_t::lower 
bool for_each_index_triangular_with_cancel_impl(tbb::task_group_context& group,
                                                Policy policy, index_t j,
                                                index_t n, Predicate f) {
  return for_with_cancel_impl(group, policy, j, n,
                              [=](index_t i) { return f(i, j); });
}

template <uplo_t Uplo, class Policy, class Predicate>
    requires Uplo == uplo_t::upper 
bool for_each_index_triangular_with_cancel_impl(tbb::task_group_context& group,
                                                Policy policy, index_t j,
                                                index_t n, Predicate f) {
  return for_with_cancel_impl(group, policy, 0, j + 1,
                              [=](index_t i) { return f(i, j); });
}
}  // namespace detail

template <uplo_t Uplo, Policy Policy, IndexPredicate<2> Functor>
  requires has_policy_v<grainularity, Policy>
bool for_each_index_triangular_with_cancel(Policy policy, index_t n,
                                           Functor f) {
  auto grainularity_outer = subdivide(get_policy<grainularity>(policy), n + 1);
  auto n_div_2 = n / 2;
  auto p = n / 2 + (n % 2);
  tbb::task_group_context group;
  return detail::for_with_cancel_impl(
      group, grainularity_outer, 0, p, [=, &group](index_t j1) {
        auto j2 = n - j1 - 1;
        if (j1 != j2) {
          detail::for_each_index_triangular_with_cancel_impl<Uplo>(
              group, policy, j1, n, f);
          detail::for_each_index_triangular_with_cancel_impl<Uplo>(
              group, policy, j2, n, f);
        } else {
          detail::for_each_index_triangular_with_cancel_impl<Uplo>(
              group, policy, j1, n, f);
        }
      });
}
} // namespace satyr
