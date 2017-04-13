#pragma once

#include <satyr/index.h>

namespace satyr {
//------------------------------------------------------------------------------
// contiguous_n_array_evaluator
//------------------------------------------------------------------------------
template <class Pointer>
class contiguous_n_array_evaluator {
 public:
  explicit contiguous_n_array_evaluator(Pointer data) : data_{data} {}

  decltype(auto) operator()(index_t index) const { return *(data_ + index); }

 private:
  Pointer data_;
};
} // namespace satyr
