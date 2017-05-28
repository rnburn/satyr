#pragma once

#include <satyr/for.h>
#include <satyr/n_array/structure.h>
#include <satyr/n_array/scalar_allocator.h>
#include <satyr/n_array/n_array_accessor.h>
#include <satyr/n_array/n_array_assignment.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/n_array_evaluator.h>
#include <satyr/n_array/n_array_view.h>
#include <satyr/n_array/n_array_subview.h>
#include <satyr/k_array.h>
#include <stdexcept>

namespace satyr {
//------------------------------------------------------------------------------
// n_array
//------------------------------------------------------------------------------
namespace detail {
template <class, class T, size_t K, class Structure>
class n_array_impl;

template <size_t... Indexes, class T, size_t K, class Structure>
class n_array_impl<std::index_sequence<Indexes...>, T, K, Structure>
    : scalar_allocator<T>,
      public n_array_cview<T, K, Structure>,
      public n_array_accessor<
          n_array_impl<std::index_sequence<Indexes...>, T, K, Structure>, K,
          Structure> {
  using base = n_array_cview<T, K, Structure>;

 public:
   using structure = Structure;

  // constructors
  n_array_impl() = default;

  template <class OtherT>
  n_array_impl(const n_array_impl<std::index_sequence<Indexes...>, OtherT, K,
                                  Structure>& other) {
    if constexpr (std::is_same_v<T, OtherT>) {
      if (data == other.data()) return;
    }
    copy_assign(other.data(), other.shape());
  }

  n_array_impl(n_array_impl&& other) noexcept {
    move_assign(other);
  }

  template <class OtherT>
  n_array_impl(const n_array_view<OtherT, K, Structure>& other) {
    copy_assign(other.data(), other.shape());
  }

  explicit n_array_impl(const satyr::shape<K>& shape) {
    if constexpr (is_equal_dimensional_v<Structure>)
      if (!is_equal_dimensional(shape))
        throw std::runtime_error{"shape must be equal dimensional"};
    T* data;
    if (get_num_elements(shape))
      data = this->allocate(get_num_elements(shape));
    else
      data = nullptr;
    static_cast<base&>(*this) = {data, shape};
  }

  explicit n_array_impl(std::enable_if_t<(Indexes, true), index_t>... extents)
    requires !is_equal_dimensional_v<Structure>
      : n_array_impl{satyr::shape<K>{extents...}} {}

  explicit n_array_impl(index_t extent) 
    requires is_equal_dimensional_v<Structure>
      : n_array_impl{satyr::shape<K>{(Indexes, extent)...}} {}

  n_array_impl(initializer_multilist<T, K> values)
      : n_array_impl{shape<K>{detail::get_extents<T, K>(values)}} {
    auto k_array = this->as_k_array();
    detail::initialize<T, K>(
        values, [=](auto... indexes) -> T& { return k_array(indexes...); });
  }

  // destructor
  ~n_array_impl() {
    if (this->data())
      this->deallocate(this->data(), get_num_elements(this->shape()));
  }

  // operator=
  n_array_impl& operator=(n_array_impl&& other) noexcept {
    move_assign(other);
    return *this;
  }

  template <class OtherT>
  n_array_impl& operator=(const n_array_impl<std::index_sequence<Indexes...>,
                                             OtherT, K, Structure>& other) {
    if constexpr (std::is_same_v<T, OtherT>) {
      if (data == other.data()) return *this;
    }
    copy_assign(other.data(), other.shape());
    return *this;
  }

  template <class OtherT>
  n_array_impl& operator=(const n_array_view<OtherT, K, Structure>& other) {
    copy_assign(other.data(), other.shape());
    return *this;
  }

  n_array_impl& operator=(detail::initializer_multilist<T, K> values) {
    auto extents_new = detail::get_extents<T, K>(values);
    reshape(extents_new);
    auto k_array = this->as_k_array();
    detail::initialize<T, K>(values, [=](auto... indexes) -> T& {
      return k_array(indexes...);
    });
    return *this; 
  }

  // accessors
  using base::data;
  T* data() { return const_cast<T*>(base::data()); }

  using base::as_k_array;
  const k_array_view<T, K>& as_k_array() {
    return reinterpret_cast<const k_array_view<T, K>&>(
        *static_cast<base*>(this));
  }

  // reshape
  void reshape(const shape<K>& shape_new) {
    if constexpr (is_equal_dimensional_v<Structure>)
      if (!is_equal_dimensional(shape_new))
        throw std::runtime_error{"shape must be equal dimensional"};
    auto num_elements = get_num_elements(this->shape());
    auto num_elements_new = get_num_elements(shape_new);
    if (num_elements == num_elements_new)
      return;
    this->deallocate(num_elements);
    auto data_new = this->allocate(num_elements_new);
    static_cast<base&>(*this) = {data_new, shape_new};
  }

  // operator()
  using n_array_accessor<n_array_impl,  K, Structure>::operator();

  // conversion
  operator n_array_subcview<T, K, Structure>() const {
    return {data(), subshape<K>{this->shape()}};
  }

  operator n_array_subview<T, K, Structure>() {
    return {data(), subshape<K>{this->shape()}};
  }

 private:
  void move_assign(n_array_impl& other) noexcept {
    if (data == other.data()) return;
    static_cast<base&>(*this) = static_cast<base&>(other);
    static_cast<base&>(other) = {nullptr, shape<K>{}};
  }

  template <class OtherT>
  void copy_assign(const OtherT* other_data, const shape<K>& other_shape) {
    reshape(other_shape);
    auto num_elements = get_num_elements(this->shape());
    auto data = this->data();
    for_(simd_v, 0, num_elements,
         [data, other_data](index_t i) { *(data + i) = *(other_data + i); });
  }
};
}

template <Scalar T, size_t K, Structure Structure = general_structure>
class n_array
    : public detail::n_array_impl<std::make_index_sequence<K>, T, K, Structure>,
      public n_array_assignment<
          n_array<T, K, Structure>,
          n_array_expression<K, Structure, flat_n_array_evaluator<T>,
                              no_policy>> {
  using base =
      detail::n_array_impl<std::make_index_sequence<K>, T, K, Structure>;

 public:
  using base::base;
  using base::operator=;
  using n_array_assignment<
      n_array,
      n_array_expression<K, Structure, flat_n_array_evaluator<T>,
                          no_policy>>::operator=;
};
} // namespace satyr
