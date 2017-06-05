#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_for.h>
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
} // namespace satyr
