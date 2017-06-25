#pragma once

#include <type_traits>

namespace satyr {
//------------------------------------------------------------------------------
// policy
//------------------------------------------------------------------------------
template <class T>
struct policy {};

//------------------------------------------------------------------------------
// is_policy_primitive_v
//------------------------------------------------------------------------------
namespace detail {
template <class T>
void is_policy_impl(policy<T>);
}

template <class T>
constexpr bool is_policy_primitive_v = false;

template <class T>
  requires requires(T t) {
    detail::is_policy_impl(t);
  } &&
  std::is_copy_constructible_v<T>
constexpr bool is_policy_primitive_v<T> = true;

//------------------------------------------------------------------------------
// has_same_policy_group_v
//------------------------------------------------------------------------------
namespace detail {
template <class T>
void has_same_policy_group_impl(policy<T>, policy<T>);
}

template <class P1, class P2>
  requires is_policy_primitive_v<P1> && is_policy_primitive_v<P2>
constexpr bool has_same_policy_group_v = false;


template <class P1, class P2>
  requires is_policy_primitive_v<P1> && is_policy_primitive_v<P2>
        && requires(P1 p1, P2 p2) {
            detail::has_same_policy_group_impl(p1, p2);
           }
constexpr bool has_same_policy_group_v<P1, P2> = true;

//------------------------------------------------------------------------------
// policy_aggregate
//------------------------------------------------------------------------------
template <class... Policies>
  requires (is_policy_primitive_v<Policies> && ...)
struct policy_aggregate 
  : Policies...
{};

//------------------------------------------------------------------------------
// is_policy_aggregate_v
//------------------------------------------------------------------------------
template <class T>
constexpr bool is_policy_aggregate_v = false;

template <class... Policies>
constexpr bool is_policy_aggregate_v<policy_aggregate<Policies...>> = true;

//------------------------------------------------------------------------------
// Policy
//------------------------------------------------------------------------------
template <class T>
concept bool Policy = is_policy_primitive_v<T> || is_policy_aggregate_v<T>;

//------------------------------------------------------------------------------
// has_policy_v
//------------------------------------------------------------------------------
namespace detail {
template <class P1, class P2>
constexpr bool has_policy_impl = std::is_same_v<P1, P2>;

template <class P1, class... Px>
constexpr bool has_policy_impl<P1, policy_aggregate<Px...>> =
  (std::is_same_v<P1, Px> || ...);
}

template <class P1, Policy P2>
  requires is_policy_primitive_v<P1>
constexpr bool has_policy_v = detail::has_policy_impl<P1, P2>;

//------------------------------------------------------------------------------
// get_policy
//------------------------------------------------------------------------------
template <class Policy>
  requires is_policy_primitive_v<Policy>
Policy get_policy(Policy policy) { return policy; }

template <class Policy, class... Policies>
  requires is_policy_primitive_v<Policy> &&
           (std::is_same_v<Policy, Policies> + ...) == 1
Policy get_policy(policy_aggregate<Policies...> policy_aggr) {
  return static_cast<Policy>(policy_aggr);
}

//------------------------------------------------------------------------------
// operator|
//------------------------------------------------------------------------------
template <class Policy1, class Policy2>
  requires is_policy_primitive_v<Policy1> &&
           is_policy_primitive_v<Policy2>
policy_aggregate<Policy1, Policy2> operator|(Policy1 policy1, Policy2 policy2) {
  return {policy1, policy2};
}

template <class Policy1, class Policy2>
  requires is_policy_primitive_v<Policy1> &&
           is_policy_primitive_v<Policy2> &&
           has_same_policy_group_v<Policy1, Policy2>
Policy2 operator|(Policy1 policy1, Policy2 policy2) {
  return policy2;
}

template <class... Policies, class Policy>
  requires is_policy_primitive_v<Policy> &&
           !(has_same_policy_group_v<Policies, Policy> || ...)
policy_aggregate<Policies..., Policy> operator|(
    policy_aggregate<Policies...> policy_aggr, Policy policy) {
  return {
    static_cast<Policies>(policy_aggr)...,
    policy
  };
}

namespace detail {
template <class P1, class P2>
  requires !has_same_policy_group_v<P1, P2>
P1 update_policy(P1 p1, P2 p2) { return p1; }

template <class P1, class P2>
  requires has_same_policy_group_v<P1, P2>
P2 update_policy(P1 p1, P2 p2) { return p2; }
}

template <class... Policies, class Policy>
  requires is_policy_primitive_v<Policy> &&
           (has_same_policy_group_v<Policies, Policy> || ...)
policy_aggregate<
  std::conditional_t<
      has_same_policy_group_v<Policies, Policy>,
          Policy, Policies>...> 
operator|(
    policy_aggregate<Policies...> policy_aggr, Policy policy) {
  return {
    detail::update_policy(
        static_cast<Policies>(policy_aggr), policy)...
  };
}

template <Policy Policy, class... Policies>
auto operator|(Policy policy, policy_aggregate<Policies...> policy_aggr) {
  return (policy | ... | static_cast<Policies>(policy_aggr));
}
} // namespace satyr
