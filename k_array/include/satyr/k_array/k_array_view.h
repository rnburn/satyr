#pragma once

#include <cstddef>
#include <type_traits>
#include <array>

#include <satyr/concept.h>
#include <satyr/k_array/shape.h>
#include <satyr/k_array/k_array_accessor.h>
#include <satyr/k_array/k_array_subview.h>

namespace satyr {
template <Semiregular T, size_t K, class Alloc = std::allocator<T>>
class k_array;

//------------------------------------------------------------------------------
// k_array_view
//------------------------------------------------------------------------------
template <class T, size_t K>
class k_array_view;

template <class T, size_t K>
  requires std::is_const_v<T> && Semiregular<std::remove_const_t<T>>
class k_array_view<T, K> 
  : public detail::k_array_const_accessor<k_array_view<T, K>, K>
{
 public:
   // constructor
   k_array_view() : data_{nullptr}, shape_{}
   {}

   k_array_view(T* data, shape<K> shape)
     : data_{data}, shape_{shape}
   {}

   // accessors
   T* data() const { return data_; }
   T* cdata() const { return data_; }

   const satyr::shape<K>& shape() const { return shape_; }

   index_t extent(index_t dimension) const { return shape_.extent(dimension); }

   // conversion
  operator k_array_subcview<T, K>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  // iteration
  T* begin() const { return data_; }
  T* end() const { return data_ + get_num_elements(*this); }
  T* cbegin() const { return begin(); }
  T* cend() const { return end(); }

 private:
   T* data_;
   satyr::shape<K> shape_;
};

template <Semiregular T, size_t K>
  requires !std::is_const_v<T>
class k_array_view<T, K>
    : public k_array_view<const T, K>,
      public detail::k_array_const_accessor<k_array_view<T, K>, K> {
  using base = k_array_view<const T, K>;
 public:
  // constructor
   k_array_view() = default;

   k_array_view(T* data, satyr::shape<K> shape)
     : base{data, shape}
   {}

   // accessors
   T* data() const { return const_cast<T*>(base::data()); }

   using detail::k_array_const_accessor<k_array_view<T, K>, K>::operator();

   // conversion
  operator k_array_subcview<T, K>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  operator k_array_subview<T, K>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  // iteration
  T* begin() const { return const_cast<T*>(base::begin()); }
  T* end() const { return const_cast<T*>(base::end()); }
};

//------------------------------------------------------------------------------
// k_array_cview
//------------------------------------------------------------------------------
template <class T, size_t K>
using k_array_cview = k_array_view<const T, K>;

//------------------------------------------------------------------------------
// make_k_array_view
//------------------------------------------------------------------------------
template <class T, size_t K>
k_array_view<T, K> make_k_array_view(T* data, const shape<K>& shape) {
  return {data, shape};
}

template <class T, size_t K>
k_array_subview<T, K> make_k_array_view(T* data, const subshape<K>& subshape) {
  return {data, subshape};
}

//------------------------------------------------------------------------------
// make_view
//------------------------------------------------------------------------------
template <class T, size_t K>
k_array_view<T, K> make_view(const k_array_view<T, K>& array) {
  return array;
}

template <class T, size_t K>
k_array_cview<T, K> make_view(const k_array<T, K>& array) {
  return {array.data(), array.shape()};
}

template <class T, size_t K>
k_array_view<T, K> make_view(k_array<T, K>& array) {
  return {array.data(), array.shape()};
}
} // namespace satyr
