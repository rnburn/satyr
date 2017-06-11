#pragma once

#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_accessor.h>
#include <satyr/n_array/n_array_assignment.h>
#include <satyr/n_array/concept.h>
#include <satyr/k_array.h>
#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_subview
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
class n_array_subview;

template <Scalar T, size_t K, Structure Structure>
  requires std::is_const_v<T>
class n_array_subview<T, K, Structure> :
    public n_array_const_accessor<n_array_subview<T, K, Structure>, K, Structure>
{
 public:
   using structure = Structure;

   // constructor
   n_array_subview() = default;

   n_array_subview(T* data, subshape<K> shape)
     : array_{data, shape}
   {}

   // accessors
   T* data() const { return array_.data(); }
   T* cdata() const { return array_.cdata(); }

   const k_array_subview<T, K>& as_k_array() const { return array_; }

   const subshape<K>& shape() const { return array_.shape(); }

 private:
  k_array_subview<T, K> array_;
};

template <Scalar T, size_t K, Structure Structure>
  requires !std::is_const_v<T>
class n_array_subview<T, K, Structure>
    : public n_array_subview<const T, K, Structure>,
      public n_array_const_accessor<n_array_subview<T, K, Structure>, K,
                                    Structure>,
      public n_array_const_assignment<
          n_array_subview<T, K, Structure>,
          n_array_expression<K, Structure, n_array_subview_evaluator<T, K>,
                             no_policy>> {
  using base = n_array_subview<const T, K, Structure>;

 public:
  // constructor
  n_array_subview() = default;

  n_array_subview(T* data, subshape<K> subshape) : base{data, subshape} {}

  // accessors
  T* data() const { return const_cast<T*>(base::data()); }

  const k_array_subview<T, K>& as_k_array() const {
    return reinterpret_cast<const k_array_subview<T, K>&>(base::as_k_array());
  }

  using n_array_const_assignment<
      n_array_subview,
      n_array_expression<K, Structure, n_array_subview_evaluator<T, K>,
                         no_policy>>::operator=;
};

//------------------------------------------------------------------------------
// n_array_subcview
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
using n_array_subcview = n_array_subview<const T, K, Structure>;

//------------------------------------------------------------------------------
// -make_n_array_subview
//------------------------------------------------------------------------------
template <Structure Structure, Scalar T, size_t K>
n_array_subview<T, K, Structure> make_n_array_subview(
    T* data, const subshape<K>& subshape) {
  return {data, subshape};
}

//------------------------------------------------------------------------------
// deconstify_n_array_subview
//------------------------------------------------------------------------------
namespace detail {
template <Scalar T, size_t K, Structure Structure>
n_array_subview<uncvref_t<T>, K, Structure> deconstify_n_array_subview(
    const n_array_subview<T, K, Structure>& array) {
  return {const_cast<uncvref_t<T>*>(array.data()), array.shape()};
}
} // namespace detail

//------------------------------------------------------------------------------
// make_view
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
n_array_subview<T, K, Structure> make_view(
    const n_array_subview<T, K, Structure>& array) {
  return array;
}
} // namespace satyr
