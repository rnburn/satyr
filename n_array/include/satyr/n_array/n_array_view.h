#pragma once

#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_accessor.h>
#include <satyr/n_array/n_array_assignment.h>
#include <satyr/n_array/n_array_subview.h>
#include <satyr/n_array/n_array_evaluator.h>
#include <satyr/k_array.h>
#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_view
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
class n_array_view;

template <Scalar T, size_t K, Structure Structure>
  requires std::is_const_v<T>
class n_array_view<T, K, Structure> :
    public n_array_const_accessor<n_array_view<T, K, Structure>, K, Structure>
{
 public:
   using structure = Structure;

   // constructor
   n_array_view() = default;

   n_array_view(T* data, shape<K> shape)
     : array_{data, shape}
   {}

   // accessors
   T* data() const { return array_.data(); }
   T* cdata() const { return array_.cdata(); }

   const k_array_view<T, K>& as_k_array() const { return array_; }

   const satyr::shape<K>& shape() const { return array_.shape(); }

   index_t extent(index_t dimension) const { return array_.extent(dimension); }

  // conversion
  operator n_array_subcview<T, K, Structure>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  // iteration
  T* begin() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return array_.begin(); 
  }

  T* end() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return array_.end(); 
  }

  T* cbegin() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return array_.cbegin(); 
  }

  T* cend() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return array_.cend(); 
  }

 private:
  k_array_view<T, K> array_;
};

template <Scalar T, size_t K, Structure Structure>
  requires !std::is_const_v<T>
class n_array_view<T, K, Structure>
    : public n_array_view<const T, K, Structure>,
      public n_array_const_accessor<n_array_view<T, K, Structure>, K,
                                    Structure>,
      public n_array_const_assignment<
          n_array_view<T, K, Structure>,
          n_array_expression<K, Structure, flat_n_array_evaluator<T>,
                             no_policy>> {
  using base = n_array_view<const T, K, Structure>;

 public:
  // constructor
  n_array_view() = default;

  n_array_view(T* data, shape<K> shape) : base{data, shape} {}

  // accessors
  T* data() const { return const_cast<T*>(base::data()); }

  const k_array_view<T, K>& as_k_array() const {
    return reinterpret_cast<const k_array_view<T, K>&>(base::as_k_array());
  }

  using n_array_const_assignment<
      n_array_view,
      n_array_expression<K, Structure, flat_n_array_evaluator<T>,
                         no_policy>>::operator=;

  using n_array_const_accessor<n_array_view<T, K, Structure>, K, Structure>::
  operator();

  // conversion
  operator n_array_subcview<T, K, Structure>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  operator n_array_subview<T, K, Structure>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  // iteration
  T* begin() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return as_k_array().begin(); 
  }

  T* end() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return as_k_array().end(); 
  }
};

//------------------------------------------------------------------------------
// n_array_cview
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
using n_array_cview = n_array_view<const T, K, Structure>;

//------------------------------------------------------------------------------
// make_view
//------------------------------------------------------------------------------
template <class T, size_t K, Structure Structure>
n_array_view<T, K, Structure> make_view(
    const n_array_view<T, K, Structure>& array) {
  return {array.data(), array.shape()};
}

template <class T, size_t K, Structure Structure>
n_array_view<T, K, Structure> make_view(n_array<T, K, Structure>& array) {
  return {array.data(), array.shape()};
}

template <class T, size_t K, Structure Structure>
n_array_cview<T, K, Structure> make_view(
    const n_array<T, K, Structure>& array) {
  return {array.data(), array.shape()};
}
}  // namespace satyr
