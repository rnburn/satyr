#pragma once

#include <satyr/index.h>
#include <satyr/execution_policy.h>
#include <cstddef>
#include <array>
#include <algorithm>

namespace satyr {
//------------------------------------------------------------------------------
// split_range
//------------------------------------------------------------------------------
constexpr struct split_range {} split_range_v;

//------------------------------------------------------------------------------
// proportional_split_range
//------------------------------------------------------------------------------
struct proportional_split_range {
  index_t left, right;
};

//------------------------------------------------------------------------------
// blocked_range
//------------------------------------------------------------------------------
class blocked_range {
 public:
   blocked_range() = default;

   blocked_range(index_t first, index_t last, index_t grainsize = 1)
       : first_{first}, last_{last}, grainsize_{grainsize} {}

   blocked_range(blocked_range& range, split_range /*split*/) {
     auto middle = static_cast<index_t>(range.first_ + range.size() / 2u);
     first_ = middle;
     last_ = range.last_;
     range.last_ = middle;
   }

   blocked_range(blocked_range& range, proportional_split_range split) {
     index_t right_split_size = static_cast<index_t>(
         static_cast<float>(range.size()) * static_cast<float>(split.right) /
             static_cast<float>(split.left + split.right) +
         0.5f);
     auto middle = static_cast<index_t>(range.last_ - right_split_size);
     first_ = middle;
     last_ = range.last_;
     range.last_ = middle;
   }

   index_t grainsize() const { return grainsize_; }

   bool empty() const { return !(first_ < last_);}

   index_t size() const { return last_ - first_; }

   bool is_divisible() const { return grainsize_ < size(); }

   index_t first() const { return first_; }

   index_t last() const { return last_; }

  private:
   index_t first_, last_;
   index_t grainsize_;
};

//------------------------------------------------------------------------------
// k_blocked_range
//------------------------------------------------------------------------------
template <size_t K>
class k_blocked_range {
 public:
   k_blocked_range() = default;

   k_blocked_range(const std::array<index_t, K>& extents, index_t grainsize) {
     std::array<index_t, K-1> cardinalities;
     index_t cardinality = 1;
     for (index_t i = 0; i < K; ++i) {
       ranges_[i] = blocked_range{
           0, extents[i],
           subdivide(satyr::grainsize{grainsize}, cardinality).value};
       cardinality *= extents[i];
     }
   }

   k_blocked_range(k_blocked_range& range, split_range split) 
     : ranges_{range.ranges_}
   {
     for (index_t i=K; i-->0;) {
       if (ranges_[i].is_divisible()) {
         ranges_[i] = blocked_range{range.ranges_[i], split};
         break;
       }
     }
   }

   k_blocked_range(k_blocked_range& range, proportional_split_range split) 
     : ranges_{range.ranges_}
   {
     for (index_t i=K; i-->0;) {
       if (ranges_[i].is_divisible()) {
         ranges_[i] = blocked_range{range.ranges_[i], split};
         break;
       }
     }
   }

   bool empty() const {
     return std::any_of(std::begin(ranges_), std::end(ranges_),
                        [](auto& range) { return range.empty(); });
   }

   bool is_divisible() const {
     return std::any_of(std::begin(ranges_), std::end(ranges_),
                        [](auto& range) { return range.is_divisible(); });
   }

   index_t size() const {
     index_t result=1;
     for (auto& range : ranges_)
       result *= range.size();
     return result;
   }

   template <size_t I>
     requires I < K
   index_t first() const {
     return ranges_[I].first();
   }

   template <size_t I>
     requires I < K
   index_t last() const {
     return ranges_[I].last();
   }
 private:
   std::array<blocked_range, K> ranges_;
};
} // namesapce satyr
