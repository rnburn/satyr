#pragma once

#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_accessor.h>
#include <satyr/k_array.h>
#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_view
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
  requires std::is_base_of_v<structure, Structure>
class n_array_view;

template <Scalar T, size_t K, Structure Structure>
  requires std::is_const_v<T> && std::is_base_of_v<structure, Structure>
class n_array_view<T, K, Structure> :
    public n_array_const_accessor<n_array_view<T, K, Structure>, K, Structure>
{
 public:
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

 private:
  k_array_view<T, K> array_;
};

template <Scalar T, size_t K, Structure Structure>
  requires !std::is_const_v<T>
class n_array_view<T, K, Structure> : n_array_view<const T, K, Structure>, 
      public n_array_const_accessor<n_array_view<T, K, Structure>, K, Structure>
{
  using base = n_array_view<const T, K, Structure>;
 public:
   // constructor
   n_array_view() = default;

   n_array_view(T* data, shape<K> shape)
     : base{data, shape} {}

   // accessors
   T* data() const { return const_cast<T*>(base::data()); }

   const k_array_view<T, K>& as_k_array() const {
     return reinterpret_cast<const k_array_view<T, K>&>(base::as_k_array());
   }
};

//------------------------------------------------------------------------------
// n_array_const_view
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
using n_array_const_view = n_array_view<const T, K, Structure>;
} // namespace satyr
