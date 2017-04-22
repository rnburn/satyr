#pragma once

#include <satyr/concept.h>
#include <satyr/index.h>
#include <satyr/n_array/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// contiguous_n_array_evaluator
//------------------------------------------------------------------------------
template <Scalar T>
class contiguous_n_array_evaluator {
 public:
  explicit contiguous_n_array_evaluator(T* data) : data_{data} {}

  decltype(auto) operator()(index_t index) const { return *(data_ + index); }

 private:
  T* data_;
};

//------------------------------------------------------------------------------
// make_evaluator
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
contiguous_n_array_evaluator<T> make_evaluator(
    n_array<T, K, Structure>& array) {
  return contiguous_n_array_evaluator{array.data()};
}

template <Scalar T, size_t K, Structure Structure>
contiguous_n_array_evaluator<T> make_evaluator(
    const n_array_view<T, K, Structure>& array) {
  return contiguous_n_array_evaluator{array.data()};
}
}  // namespace satyr
