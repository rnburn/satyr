#pragma once

#include <cstddef>
#include <type_traits>
#include <satyr/index.h>
#include <satyr/concept.h>
#include <satyr/traits.h>
#include <satyr/n_array/structure.h>

namespace satyr {
//------------------------------------------------------------------------------
// FlatEvaluator
//------------------------------------------------------------------------------
template <class T>
concept bool FlatEvaluator = 
  requires (T f, index_t i) {
    { f(i) } -> Scalar;
    &T::operator();
  } && std::is_copy_constructible_v<T>;

//------------------------------------------------------------------------------
// KEvaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class T>
struct is_k_evaluator_impl {
  static constexpr bool value = false;
};

template <size_t... Indexes, class T>
  requires requires (T f, 
                     std::enable_if_t<(Indexes,true), index_t>... indexes) {
    { f(indexes...) } -> Scalar;
    &T::operator();
  }
struct is_k_evaluator_impl<std::index_sequence<Indexes...>, T> {
  static constexpr bool value = true;
};
}

template <class T, size_t K>
concept bool KEvaluator =
    detail::is_k_evaluator_impl<std::make_index_sequence<2*K>, T>::value;

//------------------------------------------------------------------------------
// Evaluator
//------------------------------------------------------------------------------
template <class T, size_t K>
concept bool Evaluator = FlatEvaluator<T> || KEvaluator<T, K>;

//------------------------------------------------------------------------------
// Expressible
//------------------------------------------------------------------------------
template <size_t K, Structure Structure, Evaluator<K> Evaluator>
class n_array_expression;

template <Scalar T, size_t K, Structure Structure>
class n_array;

template <Scalar T, size_t K, Structure Structure>
class n_array_view;

namespace detail {
template <class>
constexpr bool match_n_array_expression = false;

template <size_t K, Structure Structure, Evaluator<K> Evaluator>
constexpr bool
    match_n_array_expression<n_array_expression<K, Structure, Evaluator>> =
        true;

template <class>
constexpr bool match_n_array = false;

template <Scalar T, size_t K, Structure Structure>
constexpr bool match_n_array<n_array<T, K, Structure>> = true;

template <class>
constexpr bool match_n_array_view = false;

template <Scalar T, size_t K, Structure Structure>
constexpr bool match_n_array_view<n_array_view<T, K, Structure>> = true;
} // namespace detail

template <class T>
concept bool NArrayExpressible = 
  detail::match_n_array_expression<uncvref_t<T>> ||
  detail::match_n_array<uncvref_t<T>> ||
  detail::match_n_array_view<uncvref_t<T>>;

template <class T>
concept bool RealNArrayExpressible =
    NArrayExpressible<T>&& RealScalar<value_type_t<T>>;

template <class T>
concept bool Expressible = NArrayExpressible<T> || Scalar<T>;

template <class T>
concept bool RealExpressible = RealNArrayExpressible<T> || RealScalar<T>;
}  // namespace satyr
