#pragma once

#include <satyr/n_array/structure.h>
#include <satyr/n_array/n_array_accessor.h>
#include <satyr/n_array/n_array_assignment.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/n_array_evaluator.h>
#include <satyr/n_array/n_array_view.h>
#include <satyr/n_array/n_array_subview.h>
#include <satyr/n_array/utility.h>
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
    : public n_array_cview<T, K, Structure>,
      public n_array_accessor<
          n_array_impl<std::index_sequence<Indexes...>, T, K, Structure>, K,
          Structure> {
  using base = n_array_cview<T, K, Structure>;
 public:
   using structure = Structure;

  // constructors
  n_array_impl() = default;

  n_array_impl(n_array_impl&& other) noexcept {
    move_assign(other);
  }

  explicit n_array_impl(const satyr::shape<K>& shape) {
    if constexpr (is_equal_dimensional_v<Structure>) {
      if (!is_equal_dimensional(shape)) {
        std::cerr << "shape must be equal dimensional\n";
        std::terminate();
      }
    }
    T* data;
    if (get_num_elements(shape))
      data = scalar_allocate<T>(get_num_elements(shape));
    else
      data = nullptr;
    static_cast<base&>(*this) = {data, shape};
  }

  explicit n_array_impl(std::enable_if_t<(Indexes, true), index_t>... extents)
    requires !is_equal_dimensional_v<Structure>
      : n_array_impl{satyr::shape<K>{extents...}} {}

  explicit n_array_impl(index_t extent) 
    requires is_equal_dimensional_v<Structure>
      : n_array_impl{satyr::shape<K>{((void)Indexes, extent)...}} {}

  n_array_impl(initializer_multilist<T, K> values)
      : n_array_impl{shape<K>{detail::get_extents<T, K>(values)}} {
    auto array = this->as_k_array();
    detail::initialize<T, K>(values, [array](auto... indexes) -> auto& {
      return array(indexes...);
    });
  }

  // destructor
  ~n_array_impl() {
    if (this->data())
      scalar_deallocate(this->data());
  }

  // operator=
  n_array_impl& operator=(n_array_impl&& other) noexcept {
    move_assign(other);
    return *this;
  }

  n_array_impl& operator=(const n_array_impl& other) {
    if (data() == other.data()) return *this;
    copy_assign(other.data(), other.shape());
    return *this;
  }

  template <class OtherT>
  n_array_impl& operator=(const n_array_impl<std::index_sequence<Indexes...>,
                                             OtherT, K, Structure>& other) {
    if constexpr (std::is_same_v<T, OtherT>) {
      if (data() == other.data()) return *this;
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
    if constexpr (is_equal_dimensional_v<Structure>) {
      if (!is_equal_dimensional(shape_new)) {
        std::cerr << "shape must be equal dimensional\n";
        std::terminate();
      }
    }
    auto num_elements = get_num_elements(this->shape());
    auto num_elements_new = get_num_elements(shape_new);
    if (num_elements == num_elements_new)
      return;
    T* data_new;
    if (num_elements_new == 0) {
      scalar_deallocate(this->data());
      data_new = nullptr;
    } else if (num_elements) {
      data_new = scalar_reallocate(this->data(), num_elements_new);
    } else {
      data_new = scalar_allocate<T>(num_elements_new);
    }
    static_cast<base&>(*this) = {data_new, shape_new};
  }

  // operator()
  using n_array_accessor<n_array_impl,  K, Structure>::operator();

  // conversion
  operator n_array_subview<T, K, Structure>() {
    return {data(), subshape<K>{this->shape()}};
  }

  operator n_array_view<T, K, Structure>() { return {data(), this->shape()}; }

  // iteration
  using base::begin;
  using base::end;

  T* begin() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return const_cast<T*>(base::begin());
  }

  T* end() const 
    requires std::is_same_v<Structure, general_structure>
  { 
    return const_cast<T*>(base::end());
  }

 private:
  void move_assign(n_array_impl& other) noexcept {
    if (data() == other.data()) return;
    static_cast<base&>(*this) = static_cast<base&>(other);
    static_cast<base&>(other) = {nullptr, shape<K>{}};
  }

  template <class OtherT, class OtherShape>
  void copy_assign(const OtherT* other_data, const OtherShape& other_shape) {
    reshape(other_shape);
    auto lhs = make_expression(
        make_n_array_view<Structure>(this->data(), this->shape()));
    auto rhs =
        make_expression(make_n_array_view<Structure>(other_data, other_shape));
    apply(equals_functor{}, lhs, rhs);
  }
};
}

template <Scalar T, size_t K, Structure Structure>
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

  // constructors
  n_array(const n_array& other)
      : n_array{static_cast<const n_array_cview<T, K, Structure>&>(other)} {}

  template <Scalar OtherT, class OtherStructure>
    requires detail::have_common_structure_v<
                  n_array, 
                  n_array_view<OtherT, K, OtherStructure>>
  n_array(const n_array_view<OtherT, K, OtherStructure>& other) {
    this->reshape(other.shape());
    *this = make_expression(other);
  }

  template <Scalar OtherT, class OtherStructure>
    requires detail::have_common_structure_v<
                  n_array, 
                  n_array_view<OtherT, K, OtherStructure>>
  n_array(const n_array_subview<OtherT, K, OtherStructure>& other) {
    this->reshape(other.shape());
    *this = make_expression(other);
  }

  n_array(n_array&& other) noexcept
      : base{static_cast<base&&>(std::move(other))} {}

  // assignment
  n_array& operator=(n_array&& other) noexcept {
    *this = static_cast<base&&>(std::move(other));
    return *this;
  }

  n_array& operator=(const n_array& other) {
    *this = make_expression(other);
    return *this;
  }
};
} // namespace satyr
