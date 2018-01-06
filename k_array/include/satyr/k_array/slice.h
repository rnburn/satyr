#pragma once

#include <satyr/index.h>
#include <satyr/traits.h>
#include <satyr/k_array/subshape.h>
#include <tuple>

namespace satyr {
//------------------------------------------------------------------------------
// range
//------------------------------------------------------------------------------
struct range {
  index_t first, last;
};

//------------------------------------------------------------------------------
// all
//------------------------------------------------------------------------------
constexpr struct all {
} all_v{};

//------------------------------------------------------------------------------
// is_free_slice_v
//------------------------------------------------------------------------------
template <class T>
constexpr bool is_free_slice_v = std::is_convertible_v<uncvref_t<T>, all> ||
                                 std::is_same_v<uncvref_t<T>, range>;

//------------------------------------------------------------------------------
// is_slice_v
//------------------------------------------------------------------------------
template <class T>
constexpr bool is_slice_v =
    is_free_slice_v<T> || std::is_convertible_v<T, index_t>;

//------------------------------------------------------------------------------
// has_free_slices_v
//------------------------------------------------------------------------------
template <class... Tx>
constexpr bool has_free_slices_v = (is_free_slice_v<Tx> || ...);

//------------------------------------------------------------------------------
// slices_as_subshape_v
//------------------------------------------------------------------------------
namespace detail {
enum class slice_kind_t {
  fixed, range, all
};

template <class T>
constexpr slice_kind_t get_slice_kind(T) {
  if (std::is_convertible_v<T, index_t>) 
    return slice_kind_t::fixed;
  if (std::is_same_v<T, range>)
    return slice_kind_t::range;
  if (std::is_same_v<T, all>) 
    return slice_kind_t::all;
}

template <class... Tx>
constexpr bool slices_as_subshape_impl(Tx... slices) {
  std::array<slice_kind_t, sizeof...(Tx)> slice_kinds = {
      get_slice_kind(slices)...};
  int last_free_index = -1;
  int num_slices = static_cast<int>(slice_kinds.size());
  for (int i=num_slices; i-->0;) {
    if (slice_kinds[i] != slice_kind_t::fixed) {
      last_free_index = i;
      break;
    }
  }
  if (last_free_index == -1) return false;
  for(int i=0; i<last_free_index; ++i) {
    if (slice_kinds[i] == slice_kind_t::fixed)
      return true;
  }

  int first_range_index = -1;
  for (int i=0; i<num_slices; ++i) {
    if (slice_kinds[i] == slice_kind_t::range) {
      first_range_index = i;
      break;
    }
  }
  if (first_range_index == -1) return false; 

  for (int i=0; i<first_range_index; ++i) {
    if (slice_kinds[i] != slice_kind_t::all)
      return true;
  }

  for (int i=first_range_index+1; i<num_slices; ++i) {
    if (slice_kinds[i] != slice_kind_t::fixed)
      return true;
  }

  return false;
}
} // namespace detail

// slices_as_subshape_v is true if the subregion of a sliced `shape` 
// must be indexed with a `subshape`.
//
// slices_as_subshape(all_v, range{1, 2}) returns false since the range can
// be indexed with a 1-dimensional `shape`.
//
// slices_as_subshape(range{1, 2}, range{1, 2}) returns true since the subregion
// must use a stride for the second index.
template <class... Tx>
  requires (is_slice_v<Tx> && ...) 
constexpr bool slices_as_subshape_v = detail::slices_as_subshape_impl(Tx{}...);

//------------------------------------------------------------------------------
// slice
//------------------------------------------------------------------------------
namespace detail {
inline index_t get_slice_offset(all) {
  return 0;
}

inline index_t get_slice_offset(range range) {
  return range.first;
}

inline index_t get_slice_offset(index_t index) {
  return index;
}

template <size_t I, size_t K>
index_t get_slice_extent(const std::array<index_t, K>& extents, all) {
  return extents[I];
}

template <size_t I, size_t K>
index_t get_slice_extent(const std::array<index_t, K>& extents, range range) {
  return range.last - range.first;
}

// slice_impl -- shape
template <size_t I, size_t J, size_t U, size_t V>
void slice_impl(const std::array<index_t, U>& extents, index_t stride,
                std::array<index_t, V>& extents_new,
                std::array<index_t, V>& strides_new, index_t& offset) {}

template <size_t I, size_t J, size_t U, size_t V, class Slice,
          class... SlicesRest>
void slice_impl(const std::array<index_t, U>& extents, index_t stride,
                std::array<index_t, V>& extents_new,
                std::array<index_t, V>& strides_new, index_t& offset,
                Slice slice, SlicesRest... slices_rest) {
  offset += stride * get_slice_offset(slice);    
  auto stride_next = stride * extents[I];
  if constexpr (is_free_slice_v<Slice>) {
    extents_new[J] = get_slice_extent<I>(extents, slice);
    strides_new[J] = stride;
    slice_impl<I+1, J+1>(extents, stride_next, extents_new, strides_new, offset,
        slices_rest...);
  } else {
    slice_impl<I+1, J>(extents, stride_next, extents_new, strides_new, offset,
        slices_rest...);
  }
}

// slice_impl -- subshape
template <size_t I, size_t J, size_t U, size_t V>
void slice_impl(const std::array<index_t, U>& extents,
                const std::array<index_t, U>& strides,
                std::array<index_t, V>& extents_new,
                std::array<index_t, V>& strides_new, index_t& offset) {}

template <size_t I, size_t J, size_t U, size_t V, class Slice,
          class... SlicesRest>
void slice_impl(const std::array<index_t, U>& extents,
                const std::array<index_t, U>& strides,
                std::array<index_t, V>& extents_new,
                std::array<index_t, V>& strides_new, index_t& offset,
                Slice slice, SlicesRest... slices_rest) {
  auto stride = strides[I];
  offset += stride*get_slice_offset(slice);
  if constexpr (is_free_slice_v<Slice>) {
    extents_new[J] = get_slice_extent<I>(extents, slice);
    strides_new[J] = stride;
    slice_impl<I + 1, J + 1>(extents, strides, extents_new, strides_new, offset,
                             slices_rest...);
  } else {
    slice_impl<I + 1, J>(extents, strides, extents_new, strides_new, offset,
                         slices_rest...);
  }
}
} // namespace detail

template <size_t K, class... Slices>
  requires K == sizeof...(Slices) &&
           (is_slice_v<Slices> && ...) &&
           has_free_slices_v<Slices...>
auto slice(const shape<K>& shape, Slices... slices) {
  constexpr size_t num_free_dimensions = (is_free_slice_v<Slices> + ...);
  std::array<index_t, num_free_dimensions> extents_new;
  std::array<index_t, num_free_dimensions> strides_new;
  index_t offset = 0;
  detail::slice_impl<0, 0>(shape.extents(), 1, extents_new,
                           strides_new, offset, slices...);
  if constexpr (slices_as_subshape_v<Slices...>)
    return std::make_tuple(
        satyr::subshape(satyr::shape(extents_new), strides_new), offset);
  else
    return std::make_tuple(satyr::shape(extents_new), offset);
}

template <size_t K, class... Slices>
  requires K == sizeof...(Slices) &&
           (is_slice_v<Slices> && ...) &&
           has_free_slices_v<Slices...>
auto slice(const subshape<K>& subshape, Slices... slices) {
  constexpr size_t num_free_dimensions = (is_free_slice_v<Slices> + ...);
  std::array<index_t, num_free_dimensions> extents_new;
  std::array<index_t, num_free_dimensions> strides_new;
  index_t offset = 0;
  detail::slice_impl<0, 0>(subshape.extents(), subshape.strides(), extents_new,
                           strides_new, offset, slices...);
  return std::make_tuple(
      satyr::subshape(satyr::shape(extents_new), strides_new), offset);
}
} // namespace satyr
