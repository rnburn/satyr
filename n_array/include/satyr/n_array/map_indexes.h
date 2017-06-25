#pragma once

#include <satyr/n_array/n_array_expression.h>

namespace satyr {
//------------------------------------------------------------------------------
// is_index_scalar_functor_v
//------------------------------------------------------------------------------
namespace detail {
template <class, class>
constexpr bool is_index_scalar_functor_impl = false;

template <size_t... Indexes, class F>
  requires std::is_copy_constructible_v<F> &&
           requires (F f, std::enable_if_t<(Indexes,true), index_t>... indexes) 
  {
    requires Scalar<decltype(f(indexes...))>;
  }
constexpr bool
    is_index_scalar_functor_impl<std::index_sequence<Indexes...>, F> = true;

template <size_t K, class F>
constexpr bool is_index_scalar_functor_v =
    is_index_scalar_functor_impl<std::make_index_sequence<K>, F>;
}  // namespace detail

//------------------------------------------------------------------------------
// map_indexes_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class>
class map_indexes_evaluator_impl;

template <size_t... Indexes, class F>
class map_indexes_evaluator_impl<std::index_sequence<Indexes...>, F> {
 public:
   explicit map_indexes_evaluator_impl(F f) : f_{f} {}

   decltype(auto) operator()(
       const shape<sizeof...(Indexes)>&,
       std::enable_if_t<(Indexes, true), index_t>... indexes) const {
     return f_(indexes...);
   }

  private:
   F f_;
};
} // namespace detail

template <size_t K, class F>
  requires detail::is_index_scalar_functor_v<K, F>
class map_indexes_evaluator
    : public detail::map_indexes_evaluator_impl<std::make_index_sequence<K>,
                                                F> {
  using base =
      detail::map_indexes_evaluator_impl<std::make_index_sequence<K>, F>;

 public:
  using base::base;
};

//------------------------------------------------------------------------------
// map_indexes
//------------------------------------------------------------------------------
template <Structure Structure, size_t K, class F>
  requires detail::is_index_scalar_functor_v<K, F>
n_array_expression<K, Structure, map_indexes_evaluator<K, F>, no_policy>
map_indexes(const shape<K>& shape, F f) {
  return make_n_array_expression<Structure>(shape,
                                            map_indexes_evaluator<K, F>{f});
}

template <size_t K, class F>
  requires detail::is_index_scalar_functor_v<K, F>
n_array_expression<K, general_structure, map_indexes_evaluator<K, F>, no_policy>
map_indexes(const shape<K>& shape, F f) {
  return map_indexes<general_structure>(shape, f);
}
} // namespace satyr
