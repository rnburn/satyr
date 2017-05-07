#pragma once

#include <satyr/linear_algebra/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// n_array_const_accessor
//------------------------------------------------------------------------------
// symmetric structure
template <class Derived>
struct n_array_const_accessor<Derived, 2, symmetric_structure> {
  const auto& operator()(access_mode::readonly, index_t i, index_t j) const {
    auto [s, t] = std::minmax(i, j);
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(t, s);
  }

  auto& operator()(access_mode::readwrite, index_t i, index_t j) const {
    auto [s, t] = std::minmax(i, j);
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(t, s);
  }

  auto& operator()(access_mode::raw, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(i, j);
  }

  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readwrite_v, i, j);
  }
};

// lower_triangular structure
template <class Derived>
struct n_array_const_accessor<Derived, 2, lower_triangular_structure> {
  auto operator()(access_mode::readonly, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    using Scalar = uncvref_t<decltype(derived.as_k_array()(0, 0))>;
    if (i < j)
      return Scalar{0};
    else
      return derived.as_k_array()(i, j);
  }

  auto& operator()(access_mode::readwrite, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(i, j);
  }

  auto& operator()(access_mode::raw, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(i, j);
  }

  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly_v, i, j);
  }
};

// upper_triangular structure
template <class Derived>
struct n_array_const_accessor<Derived, 2, upper_triangular_structure> {
  auto operator()(access_mode::readonly, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    using Scalar = uncvref_t<decltype(derived.as_k_array()(0, 0))>;
    if (i > j)
      return Scalar{0};
    else
      return derived.as_k_array()(i, j);
  }

  auto& operator()(access_mode::readwrite, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(i, j);
  }

  auto& operator()(access_mode::raw, index_t i, index_t j) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.as_k_array()(i, j);
  }

  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly_v, i, j);
  }
};
}  // namespace satyr
