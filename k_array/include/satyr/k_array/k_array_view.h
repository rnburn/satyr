#pragma once

#include <cstddef>
#include <type_traits>
#include <array>

#include <satyr/k_array/shape.h>
#include <satyr/k_array/k_array_accessor.h>
#include <satyr/k_array/k_array_subview.h>

namespace satyr {
//------------------------------------------------------------------------------
// k_array_view
//------------------------------------------------------------------------------
template <class T, size_t K>
class k_array_view;

template <class T, size_t K>
  requires std::is_const_v<T>
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

   // conversion
  operator k_array_subcview<T, K>() const {
    return {data(), subshape<K>{this->shape()}};
  }
 private:
   T* data_;
   satyr::shape<K> shape_;
};

template <class T, size_t K>
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
};

//------------------------------------------------------------------------------
// k_array_cview
//------------------------------------------------------------------------------
template <class T, size_t K>
using k_array_cview = k_array_view<const T, K>;
} // namespace satyr
