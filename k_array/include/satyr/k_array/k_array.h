#pragma once

#include <memory>

#include <satyr/k_array/k_array_view.h>

namespace satyr {
//------------------------------------------------------------------------------
// k_array
//------------------------------------------------------------------------------
template <class T, size_t K, class Alloc = std::allocator<T>>
class k_array : public k_array_const_view<T, K>,
                public detail::k_array_accessor<k_array<T, K, Alloc>, K>,
                private Alloc {
  using base = k_array_const_view<T, K>;
  using accessor = detail::k_array_accessor<k_array<T, K, Alloc>, K>;
 public:
  // constructor
  k_array() = default;

  template <class OtherT, class OtherAlloc>
  k_array(const k_array<OtherT, K, OtherAlloc>& other) {
    copy_assign(other);
  }

  k_array(k_array&& other) noexcept {
    move_assign(other);
  }

  explicit k_array(const shape<K>& shape, const Alloc& alloc = Alloc())
      : Alloc{alloc} {
    T* data;
    if (get_num_elements(shape))
      data = this->allocate(get_num_elements(shape));
    else
      data = nullptr;
    static_cast<base&>(*this) = {data, shape};
  }

  // destructor
  ~k_array() {
    if (this->data())
      this->deallocate(this->data(), get_num_elements(this->shape()));
  }

  // assignment
  k_array& operator=(const k_array& other) {
    copy_assign(other);
    return *this;
  }

  k_array& operator=(k_array&& other) noexcept {
    move_assign(other);
    return *this;
  }

  // accessor
  using base::operator();
  using accessor::operator();

  // data
  using base::data;
  T* data() {
    return const_cast<T*>(static_cast<base&>(*this).data());
  }

  // reshape
  void reshape(const shape<K>& shape_new) {
    auto num_elements = get_num_elements(this->shape());
    auto num_elements_new = get_num_elements(shape_new);
    if (num_elements == num_elements_new)
      return;
    this->deallocate(num_elements);
    auto data_new = this->allocate(num_elements_new);
    static_cast<base&>(*this) = {data_new, num_elements_new};
  }

 private:
  template <class OtherAlloc>
  void move_assign(k_array<T, K, OtherAlloc>& other) noexcept {
    static_cast<base&>(*this) = static_cast<base&>(other);
    static_cast<base&>(other) = {nullptr, shape<K>{}};
  }

  template <class OtherT, class OtherAlloc>
  void copy_assign(k_array<OtherT, K, OtherAlloc>& other) {
    reshape(other.shape());
    auto num_elements = get_num_elements(this->shape());
    for (index_t i=0; i<num_elements; ++i)
      *(data()+i) = *(other.data()+i);
  }
};
}  // namespace satyr
