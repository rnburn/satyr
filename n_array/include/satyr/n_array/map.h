#pragma once

#include <satyr/n_array/common_structure.h>
#include <satyr/n_array/common_shape.h>
#include <satyr/n_array/n_array_expression.h>
#include <satyr/n_array/conversion_evaluator.h>
#include <satyr/n_array/k_evaluator.h>
#include <satyr/n_array/expression.h>
#include <satyr/traits.h>

namespace satyr {
//------------------------------------------------------------------------------
// map_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <class, class, class Functor, class... Evaluators>
class map_evaluator_impl;

template <size_t... Indexes1, size_t... Indexes2, class Functor,
          class... Evaluators>
      requires (FlatEvaluator<Evaluators> && ...)
class map_evaluator_impl<std::index_sequence<Indexes1...>,
                         std::index_sequence<Indexes2...>, Functor,
                         Evaluators...> {
 public:
  map_evaluator_impl(Functor functor, const Evaluators&... evaluators)
      : functor_{functor}, evaluators_{evaluators...} {}

  decltype(auto) operator()(index_t index) const {
    return functor_(std::get<Indexes1>(evaluators_)(index)...);
  }

 private:
  Functor functor_;
  std::tuple<Evaluators...> evaluators_;
};

template <size_t... Indexes1, size_t... Indexes2, class Functor,
          class... Evaluators>
      requires (KEvaluator<Evaluators, sizeof...(Indexes1)> && ...)
class map_evaluator_impl<std::index_sequence<Indexes1...>,
                         std::index_sequence<Indexes2...>, Functor,
                         Evaluators...> {
 public:
  map_evaluator_impl(Functor functor, const Evaluators&... evaluators)
      : functor_{functor}, evaluators_{evaluators...} {}

  decltype(auto) operator()(
      const satyr::shape<sizeof...(Indexes2)>& shape,
      std::enable_if_t<(Indexes2, true), index_t>... indexes) const {
    return functor_(std::get<Indexes1>(evaluators_)(shape, indexes...)...);
  }

 private:
  Functor functor_;
  std::tuple<Evaluators...> evaluators_;
};

template <size_t K, class Functor, class... Evaluators>
class map_evaluator
    : public map_evaluator_impl<std::index_sequence_for<Evaluators...>,
                                std::make_index_sequence<K>,
                                Functor, Evaluators...> {
  using base = map_evaluator_impl<std::index_sequence_for<Evaluators...>,
                                  std::make_index_sequence<K>, Functor,
                                  Evaluators...>;

 public:
  map_evaluator(Functor f, const Evaluators&... evaluators)
    : base{f, evaluators...} {}

  using base::base;
};
} // namespace detail

//------------------------------------------------------------------------------
// make_map_evaluator
//------------------------------------------------------------------------------
namespace detail {
template <size_t K, class Functor, class... Evaluators>
    requires (Evaluator<Evaluators, K> && ...) &&
             ((FlatEvaluator<Evaluators> && ...) || 
              (KEvaluator<Evaluators, K> && ...))
auto make_map_evaluator(Functor functor, const Evaluators&... evaluators) {
  return map_evaluator<K, Functor, Evaluators...>{functor, evaluators...};
}

template <size_t K, class Functor, class... Evaluators>
    requires (Evaluator<Evaluators, K> && ...) &&
        // Workaround to gcc bug. It gives ambiguous overload error without 
        // this.
             !((FlatEvaluator<Evaluators> && ...) || 
              (KEvaluator<Evaluators, K> && ...))
auto make_map_evaluator(Functor functor, const Evaluators&... evaluators) {
  return make_map_evaluator<K>(functor, make_k_evaluator<K>(evaluators)...);
}
} // namespace detail

//------------------------------------------------------------------------------
// map
//------------------------------------------------------------------------------
namespace detail {
template <class... Expressibles>
constexpr bool have_common_structure_v = false;

template <class... Expressibles>
  requires requires {
    typename common_structure_t<Expressibles...>;
  }
constexpr bool have_common_structure_v<Expressibles...> = true;

template <class... Expressibles>
constexpr bool have_common_shape_v = false;

template <class... Expressibles>
  requires requires(Expressibles... expressibles) {
    get_common_shape(expressibles...);
  }
constexpr bool have_common_shape_v<Expressibles...> = true;

template <class... Expressibles>
constexpr bool have_common_evaluator_v = false;

template <class... Expressibles>
  requires requires(Expressibles... expressibles) {
    (convert_expression<common_structure_t<Expressibles...>>(
        make_expression(expressibles)), ...);
  }
constexpr bool have_common_evaluator_v<Expressibles...> = true;

template <class Structure, class Expressible>
using expressible_codomain_t =
    codomain_t<decltype(convert_expression<Structure>(
        make_expression(std::declval<Expressible>())))>;

template <class Functor, class... Expressibles>
constexpr bool is_mappable_v = false;

template <class Functor, class... Expressibles>
  requires requires(Functor f, 
                    expressible_codomain_t<
                      common_structure_t<Expressibles...>,
                      Expressibles>... values) {
    requires Scalar<uncvref_t<decltype(f(values...))>>;
  }
constexpr bool is_mappable_v<Functor, Expressibles...> = true;

template <class Functor, Expressible... Expressibles>
auto map_impl(Functor f, Expressibles&&... expressibles) {
  using Structure = common_structure_t<Expressibles...>;
  auto shape = get_common_shape(expressibles...);
  constexpr size_t K = num_dimensions_v<decltype(shape)>;
  return [&](const auto&... expressions) {
    auto policy = (expressions.policy() | ...);
    return make_n_array_expression<Structure>(
        shape, detail::make_map_evaluator<K>(
                   f, convert_expression<Structure>(expressions)...),
        policy);
  }(make_expression(expressibles)...);
}
} // namespace detail

template <class Functor, Expressible... Expressibles>
  requires std::is_copy_constructible_v<Functor> &&
           detail::have_common_structure_v<Expressibles...> && 
           detail::have_common_shape_v<Expressibles...> &&
           detail::have_common_evaluator_v<Expressibles...> &&
           detail::is_mappable_v<Functor, Expressibles...>
auto map(Functor f, Expressibles&&... expressibles) {
  return detail::map_impl(f, expressibles...);
}
} // namespace satyr
