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
// k_n_array_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class T>
class k_n_array_evaluator_impl;

template <size_t... Indexes, class T>
class k_n_array_evaluator_impl<std::index_sequence<Indexes...>, T> {
 public:
  explicit k_n_array_evaluator_impl(T* data) : data_{data} {}

  decltype(auto) operator()(
      const satyr::shape<sizeof...(Indexes)>& shape,
      std::enable_if_t<(Indexes, true), index_t>... indexes) const {
    return *(data_ + get_1d_index(shape, indexes...));
  }

 private:
  T* data_;
};
}

template <Scalar T, size_t K>
class k_n_array_evaluator
    : public detail::k_n_array_evaluator_impl<std::make_index_sequence<K>,
                                      T> {
  using base = detail::k_n_array_evaluator_impl<std::make_index_sequence<K>, T>;

 public:
  explicit k_n_array_evaluator(T* data)
      : base{data} {}

  using base::base;
};
}  // namespace satyr
