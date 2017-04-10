#pragma once

#include <satyr/n_array/structure.h>
#include <satyr/n_array/scalar_allocator.h>
#include <satyr/k_array.h>

namespace satyr {
namespace detail {
//------------------------------------------------------------------------------
// n_array
//------------------------------------------------------------------------------
template <class, class T, size_t K, class Structure>
class n_array_impl {};

template <size_t... Indexes, class T, size_t K, class Structure>
class n_array_impl<
  std::index_sequence<Indexes...>,
  T, K, Structure>
{
 public:
   using structure = Structure;

  // constructors
  explicit n_array_impl(shape<K> shape)
   : array_{shape}
  {}

  explicit n_array_impl(std::enable_if_t<(Indexes, true), index_t>... extents) 
    : n_array_impl{shape<K>{extents...}}
  {}

  explicit n_array_impl(index_t extent)
    requires is_equal_dimensional_v<Structure>
   : n_array_impl{shape<K>{(Indexes,extent)...}}
  {}

  // accessors
  k_array<T, K, scalar_allocator<T>>& as_k_array() { return array_; }
  const k_array<T, K, scalar_allocator<T>>& as_k_array() const {
    return array_;
  }

  const shape<K>& shape() const { return array_.shape(); }

 private:
   k_array<T, K, scalar_allocator<T>> array_;
};
}

template <class T, size_t K, Structure Structure>
class n_array : public detail::n_array_impl<std::make_index_sequence<K>, T, K,
                                            Structure> {
 public:
  using detail::n_array_impl<std::make_index_sequence<K>, T, K,
                             Structure>::n_array_impl;
};
} // namespace satyr
