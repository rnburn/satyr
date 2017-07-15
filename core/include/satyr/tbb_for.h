#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_for.h>
#include <satyr/serial_for_each_index.h>
#include <satyr/matrix.h>
#include <tbb/parallel_for.h>
#include <tbb/task.h>

namespace satyr {
//------------------------------------------------------------------------------
// for_
//------------------------------------------------------------------------------
template <Policy Policy, IndexFunctor<1> F>
  requires has_policy_v<grainsize, Policy>
void for_(Policy policy, index_t first, index_t last, F f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  tbb::parallel_for(
      tbb::blocked_range<index_t>{first, last,
                                  static_cast<size_t>(grainsize.value)},
      [=](const tbb::blocked_range<index_t>& range) {
        for_(policy | serial_v, range.begin(), range.end(), f);
      });
}

//------------------------------------------------------------------------------
// for_each_index
//------------------------------------------------------------------------------
namespace detail {
template <size_t K>
struct tbb_k_blocked_range {
  explicit tbb_k_blocked_range(const k_blocked_range<K>& range_)
      : range{range_} {}

  tbb_k_blocked_range(
      tbb_k_blocked_range& other, 
      tbb::split /*split*/)
    : range{other.range, split_range_v} {}

  static const bool is_splittable_in_proportion = true;

  tbb_k_blocked_range(tbb_k_blocked_range& other,
                      const tbb::proportional_split& split)
      : range{other.range,
              proportional_split_range{split.left(), split.right()}} {}

  bool empty() const { return range.empty(); }

  bool is_divisible() const { return range.is_divisible(); }

  k_blocked_range<K> range;
};
} // namespace detail

template <Policy Policy, size_t K, IndexFunctor<K> Functor>
  requires has_policy_v<grainsize, Policy>
void for_each_index(Policy policy, std::array<index_t, K> extents, Functor f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = k_blocked_range(extents, grainsize.value);
  tbb::parallel_for(detail::tbb_k_blocked_range(range), [=](const auto& range) {
    for_each_index(policy | serial_v, range.range, f);
  });
}

//------------------------------------------------------------------------------
// for_with_exit
//------------------------------------------------------------------------------
namespace detail {
template <Policy Policy, IndexPredicate<1> F>
  requires has_policy_v<grainsize, Policy>
void for_with_exit_impl(tbb::task_group_context& group, Policy policy,
                          index_t first, index_t last, F f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  tbb::parallel_for(
      tbb::blocked_range<index_t>{first, last,
                                  static_cast<size_t>(grainsize.value)},
      [=, &group](const tbb::blocked_range<index_t>& range) {
        if (!for_with_exit(policy | serial_v, range.begin(), range.end(), f))
          group.cancel_group_execution();
      },
      tbb::auto_partitioner{}, group);
}
} // namespace detail

template <Policy Policy, IndexPredicate<1> F>
  requires has_policy_v<grainsize, Policy>
bool for_with_exit(Policy policy, index_t first, index_t last, F f) {
  tbb::task_group_context group;
  return detail::for_with_exit_impl(group, policy, first, last, f);
  return !group.is_group_execution_cancelled();
}

//------------------------------------------------------------------------------
// for_each_index_triangular_with_exit
//------------------------------------------------------------------------------
namespace detail {
template <uplo_t Uplo, class Policy, class Predicate>
    requires Uplo == uplo_t::lower 
void for_each_index_triangular_with_exit_impl(tbb::task_group_context& group,
                                                Policy policy, index_t j,
                                                index_t n, Predicate f) {
  for_with_exit_impl(group, policy, j, n, [=](index_t i) { return f(i, j); });
}

template <uplo_t Uplo, class Policy, class Predicate>
    requires Uplo == uplo_t::upper 
void for_each_index_triangular_with_exit_impl(tbb::task_group_context& group,
                                                Policy policy, index_t j,
                                                index_t n, Predicate f) {
  for_with_exit_impl(group, policy, 0, j + 1,
                       [=](index_t i) { return f(i, j); });
}
}  // namespace detail

template <uplo_t Uplo, Policy Policy, IndexPredicate<2> Functor>
  requires has_policy_v<grainsize, Policy>
bool for_each_index_triangular_with_exit(Policy policy, index_t n,
                                           Functor f) {
  auto grainsize_outer = subdivide(get_policy<grainsize>(policy), n + 1);
  auto n_div_2 = n / 2;
  auto p = n / 2 + (n % 2);
  tbb::task_group_context group;
  detail::for_with_exit_impl(
      group, grainsize_outer, 0, p, [=, &group](index_t j1) {
        auto j2 = n - j1 - 1;
        if (j1 != j2) {
          detail::for_each_index_triangular_with_exit_impl<Uplo>(
              group, policy, j1, n, f);
          detail::for_each_index_triangular_with_exit_impl<Uplo>(
              group, policy, j2, n, f);
        } else {
          detail::for_each_index_triangular_with_exit_impl<Uplo>(
              group, policy, j1, n, f);
        }
      });
  return !group.is_group_execution_cancelled();
}
} // namespace satyr
