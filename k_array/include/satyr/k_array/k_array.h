#pragma once

#include <memory>

#include <satyr/k_array/k_array_view.h>

namespace satyr {
//------------------------------------------------------------------------------
// k_array
//------------------------------------------------------------------------------
template <class T, size_t K, class Alloc = std::allocator<T>>
class k_array : public k_array_view<T, K>, private Alloc {
 public:
  // constructor
  k_array() = default;

  template <class OtherT, class OtherAlloc>
  k_array(const k_array<OtherT, K, OtherAlloc>& other) {
  }

  k_array(k_array&& other) {
  }

  explicit k_array(const shape<K>& shape, const Alloc& alloc = Alloc())
      : Alloc{alloc} {
    T* data;
    if (get_num_elements(shape))
      data = this->allocate(get_num_elements(shape));
    else
      data = nullptr;
    static_cast<k_array_view<T, K>&>(*this) = {data, shape};
  }

  // destructor
  ~k_array() {
    if (this->data())
      this->deallocate(this->data(), get_num_elements(this->shape()));
  }

  // assignment
  k_array& operator=(const k_array& other) {
    return *this;
  }

  k_array& operator=(k_array&& other) {
    return *this;
  }

  // reshape
  void reshape(const shape<K>& shape_new) {
  }
};
}  // namespace satyr
