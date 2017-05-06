#pragma once

#include <satyr/concept.h>
#include <satyr/execution_policy.h>
#include <satyr/serial_for.h>
#include <tbb/parallel_for.h>

namespace satyr {
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
} // namespace satyr
