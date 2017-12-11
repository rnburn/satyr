#pragma once

#include <satyr/k_array/shape.h>

namespace satyr {
//------------------------------------------------------------------------------
// get_strides
//------------------------------------------------------------------------------
namespace detail {
template <size_t K>
std::array<index_t, K> get_strides(const shape<K>& shape) {
  auto& extents = shape.extents();
  std::array<index_t, K> result;
  result[0] = 1;
  for (index_t i = 1; i < static_cast<index_t>(K); ++i)
    result[i] = result[i - 1] * extents[i - 1];
  return result;
}
} // namespace detail

//------------------------------------------------------------------------------
// subshape
//------------------------------------------------------------------------------
template <size_t K>
class subshape : public shape<K> {
 public:
  subshape() = default;

  subshape(const shape<K>& shape)
      : shape<K>{shape}, strides_{detail::get_strides(shape)} {}

  subshape(const shape<K>& shape, const std::array<index_t, K>& strides)
      : shape<K>{shape}, strides_{strides} {}

  const std::array<index_t, K>& strides() const { return strides_; }

 private:
  std::array<index_t, K> strides_;
};

//------------------------------------------------------------------------------
// get_stride
//------------------------------------------------------------------------------
template <size_t I, size_t K>
  requires I < K
index_t get_stride(const subshape<K>& subshape) {
  return std::get<I>(subshape.strides());
}

//------------------------------------------------------------------------------
// get_subshape_1d_index
//------------------------------------------------------------------------------
namespace detail {
template <size_t I, size_t K, class IndexFirst, class... IndexesRest>
index_t get_subshape_1d_index_impl(const std::array<index_t, K>& strides, 
                                   IndexFirst index_first,
                                   IndexesRest... indexes_rest) {
  if constexpr (sizeof...(IndexesRest)) 
    return index_first*std::get<I>(strides) +
              get_subshape_1d_index_impl<I + 1>(strides, indexes_rest...);
  else
    return index_first * std::get<I>(strides);
}
} // namespace detail

template <size_t K, class... Indexes>
  requires K == sizeof...(Indexes) && 
           (std::is_convertible_v<Indexes, index_t> && ...)
index_t get_subshape_1d_index(const std::array<index_t, K>& strides,
                              Indexes... indexes) {
  return detail::get_subshape_1d_index_impl<0>(strides, indexes...);
}

//------------------------------------------------------------------------------
// get_1d_index
//------------------------------------------------------------------------------
template <size_t K, class... Indexes>
  requires K == sizeof...(Indexes) && 
           (std::is_convertible_v<Indexes, index_t> && ...)
index_t get_1d_index(const subshape<K>& subshape, Indexes... indexes) {
  return get_subshape_1d_index(subshape.strides(), indexes...);
} 
} // namespace satyr

//------------------------------------------------------------------------------
// tuple_size
//------------------------------------------------------------------------------
namespace std {
template <size_t K>
struct tuple_size<satyr::subshape<K>> {
  static const size_t value = K;
};
} // namespace std

//------------------------------------------------------------------------------
// tuple_element
//------------------------------------------------------------------------------
namespace std {
template <size_t I, size_t K>
  requires I < K
struct tuple_element<I, satyr::subshape<K>> {
  using type = satyr::index_t;
};
} // namespace std
