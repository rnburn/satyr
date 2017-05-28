#pragma once

#include <satyr/concept.h>
#include <satyr/index.h>
#include <satyr/n_array/n_array.h>

namespace satyr {
//------------------------------------------------------------------------------
// flat_n_array_evaluator
//------------------------------------------------------------------------------
template <Scalar T>
class flat_n_array_evaluator {
 public:
  explicit flat_n_array_evaluator(T* data) : data_{data} {}

  T& operator()(index_t index) const { return *(data_ + index); }

 private:
  T* data_;
};

//------------------------------------------------------------------------------
// n_array_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class T>
class n_array_evaluator_impl;

template <size_t... Indexes, class T>
class n_array_evaluator_impl<std::index_sequence<Indexes...>, T> {
 public:
  explicit n_array_evaluator_impl(T* data) : data_{data} {}

  T& operator()(const satyr::shape<sizeof...(Indexes)>& shape,
                std::enable_if_t<(Indexes, true), index_t>... indexes) const {
    return *(data_ + get_1d_index(shape, indexes...));
  }

 private:
  T* data_;
};
} // namespace detail

template <Scalar T, size_t K>
class n_array_evaluator
    : public detail::n_array_evaluator_impl<std::make_index_sequence<K>,
                                      T> {
  using base = detail::n_array_evaluator_impl<std::make_index_sequence<K>, T>;

 public:
  explicit n_array_evaluator(T* data)
      : base{data} {}

  using base::base;
};

//------------------------------------------------------------------------------
// n_array_subview_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class T>
class n_array_subview_evaluator_impl;

template <size_t... Indexes, class T>
class n_array_subview_evaluator_impl<std::index_sequence<Indexes...>, T> {
 public:
  n_array_subview_evaluator_impl(
      T* data, const std::array<index_t, sizeof...(Indexes)>& strides)
      : data_{data}, strides_{strides} {}

  T& operator()(const shape<sizeof...(Indexes)>&,
                std::enable_if_t<(Indexes, true), index_t>... indexes) const {
    return *(data_ + get_subshape_1d_index(strides_, indexes...));
  }
 private:
   T* data_;
   std::array<index_t, sizeof...(Indexes)> strides_;
};
} // namespace detail

template <Scalar T, size_t K>
class n_array_subview_evaluator
    : public detail::n_array_subview_evaluator_impl<std::make_index_sequence<K>,
                                                    T> {
  using base =
      detail::n_array_subview_evaluator_impl<std::make_index_sequence<K>, T>;

 public:
  n_array_subview_evaluator(T* data, const std::array<index_t, K>& strides)
      : base{data, strides} {}
};
}  // namespace satyr
