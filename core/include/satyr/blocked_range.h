#pragma once

#include <satyr/index.h>
#include <satyr/matrix.h>
#include <satyr/execution_policy.h>
#include <cstddef>
#include <array>
#include <cmath>
#include <algorithm>
#include <cassert>

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

//------------------------------------------------------------------------------
// triangular_blocked_range
//------------------------------------------------------------------------------
template <uplo_t Uplo>
class triangular_blocked_range {
 public:
  triangular_blocked_range(index_t n, index_t grainsize)
      : n_{n}, grainsize_{grainsize}, is_column_range_{true} {
    column_range_ = column_range{0, n};
  }

  triangular_blocked_range(triangular_blocked_range& range, split_range split) {
    do_split(range, split);
  }

  triangular_blocked_range(triangular_blocked_range& range,
                           proportional_split_range split) {
    do_split(range, split);
  }

  bool is_divisible() const { return grainsize_ < size(); }

  bool empty() const { return n_ == 0; }

  index_t size() const {
    if (is_column_range_)
      return column_range_.size();
    else
      return column_slice_.size();
  }

  bool is_column_range() const { return is_column_range_; }

  std::array<index_t, 2> columns() const {
    return {column_range_.column_first, column_range_.column_last};
  }

  std::array<index_t, 3> column_rows() const {
    return {column_slice_.column, column_slice_.row_first,
            column_slice_.row_last};
  }

 private:
  struct column_range {
    index_t column_first, column_last;

    index_t size() const {
      auto delta = column_last - column_first;
      index_t a;
      if constexpr(Uplo == uplo_t::lower) 
        a = n_ - column_last;
      else
        a = column_first;
      return delta * a + delta * (delta + 1) / 2;
    }
  };

  struct column_slice {
    index_t column, row_first, row_last;

    index_t size() const { 
      return row_last - row_first;
    }
  };

  index_t n_;
  index_t grainsize_;
  bool is_column_range_;
  union {
    column_range column_range_;
    column_slice column_slice_;
  };

  index_t compute_split_column(index_t column_first, index_t column_last,
                               float target) const 
    requires Uplo == uplo_t::lower
  {
    index_t delta = column_last - column_first;
    assert(delta > 1);
    if (delta == 2) return column_first + 1;
    auto a = static_cast<float>(n_ - column_first + 1) - 0.5f;
    auto t = a - std::sqrt(a * a - 2 * target);
    auto split_column = column_first + static_cast<index_t>(std::round(t));
    return std::clamp(split_column, column_first + 1, column_last - 1);
  }

  index_t compute_split_column(index_t column_first, index_t column_last,
                               float target) const 
    requires Uplo == uplo_t::upper
  {
    index_t delta = column_last - column_first;
    assert(delta > 1);
    if (delta == 2) return column_first + 1;
    auto a = static_cast<float>(column_first) + 0.5f;
    auto t = std::sqrt(a*a+2*target) - a;
    auto split_column = column_first + static_cast<index_t>(std::round(t));
    return std::clamp(split_column, column_first + 1, column_last - 1);
  }

  index_t row_first(index_t column) const {
    if constexpr (Uplo == uplo_t::lower) {
      return column;
    } else {
      return 0;
    }
  }

  index_t row_last(index_t column) const {
    if constexpr (Uplo == uplo_t::lower) {
      return n_;
    } else {
      return column + 1;
    }
  }

  template <class Split>
  void do_split(triangular_blocked_range& range, Split split) {
    n_ = range.n_;
    grainsize_ = range.grainsize_;
    if (range.is_column_range_) {
      if (column_range_.column_first + 1 == column_range_.column_last) {
        range.is_column_range_ = false;
        auto column = range.column_first;
        range.column_slice_ = {column, row_first(column), row_last(column)};
        do_split_impl(range.column_slice_, split);
      } else {
        do_split_impl(range.column_range_, split);
      }
    } else {
      do_split_impl(range.column_slice_, split);
    }
    is_column_range_ = range.is_column_range_;
  }

  void do_split_impl(column_range& region, split_range /*split*/) {
    do_split_impl(region, region.size() / 2.0f);
  }

  void do_split_impl(column_range& region, proportional_split_range split) {
    do_split(region,
             static_cast<float>(region.size()) *
                 static_cast<float>(split.left) /
                 static_cast<float>(split.left + split.right));
  }

  void do_split_impl(column_range& region, float target) {
    auto split_column =
        compute_split_column(region.column_first, region.column_last, target);
    column_range_ = {split_column, region.column_last};
    region.column_last = split_column;
  }

  template <class Split>
  void do_split_impl(column_slice& region, Split split) {
    blocked_range left{
        blocked_range{region.row_first, region.row_last, grainsize_}, split};
    column_slice_ = {region.column, left.first(), left.last()};
    region.row_last = left.first();
  }
};
} // namesapce satyr
