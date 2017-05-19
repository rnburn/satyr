#pragma once

#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_accessor.h>
#include <satyr/n_array/n_array_assignment.h>
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

   n_array_subview(T* data, subshape<K> subshape)
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
          n_array_expression<K, Structure, contiguous_n_array_evaluator<T>,
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
      n_array_expression<K, Structure, contiguous_n_array_evaluator<T>,
                         no_policy>>::operator=;
};

//------------------------------------------------------------------------------
// n_array_csubview
//------------------------------------------------------------------------------
template <Scalar T, size_t K, Structure Structure>
using n_array_csubview = n_array_subview<const T, K, Structure>;
} // namespace satyr
