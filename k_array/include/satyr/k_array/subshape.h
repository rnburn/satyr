#pragma once

#include <satyr/k_array/shape.h>

namespace satyr {
//------------------------------------------------------------------------------
// subshape
//------------------------------------------------------------------------------
template <size_t K>
class subshape : public shape<K> {
 public:
  subshape() = default;

  subshape(const shape<K>& shape, const std::array<index_t, K>& strides)
      : shape<K>{shape}, strides_{strides} {}

  const std::array<index_t, K>& strides() const { return strides_; }

 private:
  std::array<index_t, K> strides_;
};

//------------------------------------------------------------------------------
// get_1d_index
//------------------------------------------------------------------------------
namespace detail {
template <size_t I, size_t K, class IndexFirst, class... IndexesRest>
index_t get_1d_index_impl(const subshape<K>& subshape, IndexFirst index_first,
                          IndexesRest... indexes_rest) {
  if constexpr(sizeof...(IndexesRest)) 
    return index_first*std::get<I>(subshape.strides()) +
              get_1d_index_impl<I + 1>(subshape, indexes_rest...);
  else
    return index_first * std::get<I>(subshape.strides());
}
}

template <size_t K, class... Indexes>
  requires K == sizeof...(Indexes) && 
           (std::is_convertible_v<Indexes, index_t> && ...)
index_t get_1d_index(const subshape<K>& subshape, Indexes... indexes) {
  return detail::get_1d_index_impl<0>(subshape, indexes...);
} 
} // namespace satyr
