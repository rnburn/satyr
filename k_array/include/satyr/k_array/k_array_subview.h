#pragma once

#include <cstddef>
#include <type_traits>
#include <array>
#include <cassert>

#include <satyr/concept.h>
#include <satyr/k_array/shape.h>
#include <satyr/k_array/k_array_accessor.h>

namespace satyr {
//------------------------------------------------------------------------------
// k_array_subview
//------------------------------------------------------------------------------
template <class T, size_t K>
class k_array_subview;

template <class T, size_t K>
  requires std::is_const_v<T> && Semiregular<std::remove_const_t<T>>
class k_array_subview<T, K> 
  : public detail::k_array_const_accessor<k_array_subview<T, K>, K>
{
 public:
   // constructor
  k_array_subview() : data_{nullptr}, subshape_{} {}

  k_array_subview(T* data, const subshape<K>& subshape)
      : data_{data}, subshape_{subshape} {}

  // accessors
  T* data() const { return data_; }
  T* cdata() const { return data_; }

  const subshape<K>& shape() const { return subshape_; }

  index_t extent(index_t dimension) const {
    return subshape_.extent(dimension);
  }

 private:
  T* data_;
  subshape<K> subshape_;
};

template <Semiregular T, size_t K>
  requires !std::is_const_v<T>
class k_array_subview<T, K> 
  : public k_array_subview<const T, K>,
    public detail::k_array_const_accessor<k_array_subview<T, K>, K> {
  using base = k_array_subview<const T, K>;
 public:
  // constructor
  k_array_subview() = default;

  k_array_subview(T* data, const subshape<K>& subshape)
      : base{data, subshape} {}

  // accessors
  T* data() const { return const_cast<T*>(base::data()); }

  using detail::k_array_const_accessor<k_array_subview<T, K>, K>::operator();
};

//------------------------------------------------------------------------------
// k_array_csubview
//------------------------------------------------------------------------------
template <class T, size_t K>
using k_array_subcview = k_array_subview<const T, K>;

//------------------------------------------------------------------------------
// make_k_array_subview
//------------------------------------------------------------------------------
template <class T, size_t K>
k_array_subview<T, K> make_k_array_subview(T* data,
                                           const subshape<K>& subshape) {
  return {data, subshape};
}

//------------------------------------------------------------------------------
// make_view
//------------------------------------------------------------------------------
template <class T, size_t K>
k_array_subview<T, K> make_view(const k_array_subview<T, K>& array) {
  return array;
}
} // namespace satyr
