#pragma once

#include <cstddef>
#include <array>
#include <tuple>
#include <satyr/index.h>

namespace satyr {
//------------------------------------------------------------------------------
// shape
//------------------------------------------------------------------------------
template <size_t K>
class shape {
 public:
  // constructor
  shape() {
    extents_ = {};
  }

  template <class... Extents>
    requires sizeof...(Extents) == K && 
             (std::is_convertible_v<Extents, index_t> && ...)
  shape(Extents... extents) :
    extents_{extents...}
  {}

  explicit shape(const std::array<index_t, K>& extents) : extents_{extents} {}

  // accessors
  const std::array<index_t, K> extents() const { return extents_; }
 private:
   std::array<index_t, K> extents_;
};

//------------------------------------------------------------------------------
// is_equal_dimensional
//------------------------------------------------------------------------------
template <size_t K>
bool is_equal_dimensional(const shape<K>& shape) {
  const auto& extents = shape.extents();
  for (index_t i = 1; i < K; ++i)
    if (extents[0] != extents[i]) return false;
  return true;
}

//------------------------------------------------------------------------------
// operator==
//------------------------------------------------------------------------------
template <size_t K>
bool operator==(const shape<K>& lhs, const shape<K>& rhs) {
  return lhs.extents() == rhs.extents();
}

//------------------------------------------------------------------------------
// operator!=
//------------------------------------------------------------------------------
template <size_t K>
bool operator!=(const shape<K>& lhs, const shape<K>& rhs) {
  return lhs.extents() != rhs.extents();
}

//------------------------------------------------------------------------------
// get_extent
//------------------------------------------------------------------------------
template <size_t I, size_t K>
  requires I < K
index_t get_extent(const shape<K>& shape) {
  return std::get<I>(shape.extents());
}

//------------------------------------------------------------------------------
// get_extent
//------------------------------------------------------------------------------
template <size_t I, size_t K>
  requires I < K
satyr::index_t get(const satyr::shape<K>& shape) {
  return satyr::get_extent<I>(shape);
}

//------------------------------------------------------------------------------
// get_num_elements
//------------------------------------------------------------------------------
template <size_t K>
index_t get_num_elements(const shape<K>& shape) {
  index_t result = 1;
  const auto& extents = shape.extents();
  for (index_t i=0; i<K; ++i)
    result *= extents[i];
  return result;
}

//------------------------------------------------------------------------------
// get_1d_index
//------------------------------------------------------------------------------
namespace detail {
template <size_t I, size_t K, class IndexFirst, class... IndexesRest>
index_t get_1d_index_impl(const shape<K>& shape, IndexFirst index_first,
                          IndexesRest... indexes_rest) {
  if constexpr(sizeof...(IndexesRest)) 
    return index_first + get_extent<I>(shape) * 
        get_1d_index_impl<I + 1>(shape, indexes_rest...);
  else
    return index_first;
}
}

template <size_t K, class... Indexes>
  requires K == sizeof...(Indexes) && 
           (std::is_convertible_v<Indexes, index_t> && ...)
index_t get_1d_index(const shape<K>& shape, Indexes... indexes) {
  return detail::get_1d_index_impl<0>(shape, indexes...);
}
} // namespace satyr

//------------------------------------------------------------------------------
// tuple_size
//------------------------------------------------------------------------------
namespace std {
template <size_t K>
struct tuple_size<satyr::shape<K>> {
  static const size_t value = K;
};
} // namespace std

//------------------------------------------------------------------------------
// tuple_element
//------------------------------------------------------------------------------
namespace std {
template <size_t I, size_t K>
  requires I < K
struct tuple_element<I, satyr::shape<K>> {
  using type = satyr::index_t;
};
} // namespace std
