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
template <class Range>
struct tbb_blocked_range {
  explicit tbb_blocked_range(const Range& range_)
      : range{range_} {}

  tbb_blocked_range(
      tbb_blocked_range& other, 
      tbb::split /*split*/)
    : range{other.range, split_range_v} {}

  static const bool is_splittable_in_proportion = true;

  tbb_blocked_range(tbb_blocked_range& other,
                      const tbb::proportional_split& split)
      : range{other.range,
              proportional_split_range{split.left(), split.right()}} {}

  bool empty() const { return range.empty(); }

  bool is_divisible() const { return range.is_divisible(); }

  Range range;
};
} // namespace detail

template <Policy Policy, size_t K, IndexFunctor<K> Functor>
  requires has_policy_v<grainsize, Policy>
void for_each_index(Policy policy, std::array<index_t, K> extents, Functor f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = k_blocked_range(extents, grainsize.value);
  tbb::parallel_for(detail::tbb_blocked_range(range), [=](const auto& range) {
    for_each_index(policy | serial_v, range.range, f);
  });
}

//------------------------------------------------------------------------------
// for_each_index_triangular
//------------------------------------------------------------------------------
template <uplo_t Uplo, Policy Policy, IndexFunctor<2> Functor>
  requires has_policy_v<grainsize, Policy>
void for_each_index_triangular(Policy policy, index_t n, Functor f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = triangular_blocked_range<Uplo>{n, grainsize};
  tbb::parallel_for(detail::tbb_blocked_range(range), [=](const auto& range) {
    for_each_index_triangular<Uplo>(policy | serial_v, range.range, f);
  });
}

//------------------------------------------------------------------------------
// for_with_exit
//------------------------------------------------------------------------------
template <Policy Policy, IndexPredicate<1> F>
  requires has_policy_v<grainsize, Policy>
bool for_with_exit(Policy policy, index_t first, index_t last, F f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = tbb::blocked_range<index_t>{
      first, last, static_cast<size_t>(grainsize.value)};
  if (!range.is_divisible())
    return for_with_exit(policy | serial_v, first, last, f);
  tbb::task_group_context group;
  tbb::parallel_for(
      range,
      [=, &group](const tbb::blocked_range<index_t>& range) {
        if (!for_with_exit(policy | serial_v, range.begin(), range.end(), f))
          group.cancel_group_execution();
      },
      tbb::auto_partitioner{}, group);
  return !group.is_group_execution_cancelled();
}

//------------------------------------------------------------------------------
// for_each_index_with_exit
//------------------------------------------------------------------------------
template <Policy Policy, size_t K, IndexPredicate<K> Predicate>
  requires has_policy_v<grainsize, Policy>
bool for_each_index_with_exit(Policy policy, std::array<index_t, K> extents,
    Predicate f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = k_blocked_range(extents, static_cast<size_t>(grainsize.value));
  if (!range.is_divisible())
    return for_each_index_with_exit(policy | serial_v, extents, f);
  tbb::task_group_context group;
  tbb::parallel_for(
      range,
      [=, &group](const auto& range) {
        if (!for_each_index_with_exit(policy | serial_v, range, f))
          group.cancel_group_execution();
      },
      tbb::auto_partitioner{}, group);
  return !group.is_group_execution_cancelled();
}

//------------------------------------------------------------------------------
// for_each_index_triangular_with_exit
//------------------------------------------------------------------------------
template <Policy Policy, uplo_t Uplo, IndexPredicate<2> Predicate>
  requires has_policy_v<grainsize, Policy>
bool for_each_index_triangular_with_exit(Policy policy, index_t n,
                                         Predicate f) {
  auto grainsize = get_policy<satyr::grainsize>(policy);
  auto range = triangular_blocked_range<Uplo>{n, grainsize};
  if (!range.is_divisible())
    return for_each_index_triangular_with_exit<Uplo>(policy | serial_v, n, f);
  tbb::task_group_context group;
  tbb::parallel_for(detail::tbb_blocked_range(range),
                    [=, &group](const auto& range) {
                      if (!for_each_index_triangular_with_exit<Uplo>(
                              policy | serial_v, range.range, f))
                        group.cancel_group_execution();
                    });
  return !group.is_group_execution_cancelled();
}
} // namespace satyr
