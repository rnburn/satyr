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
template <class T>
constexpr bool is_policy_primitive_v = 
  std::is_base_of_v<policy<T>, T> &&
  std::is_copy_constructible_v<T>;
  

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
template <class Policy>
  requires is_policy_primitive_v<Policy>
Policy operator|(Policy policy1, Policy policy2) {
  return policy2;
}

template <class Policy1, class Policy2>
  requires is_policy_primitive_v<Policy1> &&
           is_policy_primitive_v<Policy2>
policy_aggregate<Policy1, Policy2> operator|(Policy1 policy1, Policy2 policy2) {
  return {policy1, policy2};
}

template <class... Policies, class Policy>
  requires is_policy_primitive_v<Policy> &&
           !(std::is_same_v<Policies, Policy> && ...)
policy_aggregate<Policies..., Policy> operator|(
    policy_aggregate<Policies...> policy_aggr, Policy policy) {
  return {
    static_cast<Policies>(policy_aggr)...,
    policy
  };
}

template <class... Policies, class Policy>
  requires is_policy_primitive_v<Policy> &&
           (std::is_same_v<Policies, Policy> || ...)
policy_aggregate<Policies...> operator|(
    policy_aggregate<Policies...> policy_aggr, Policy policy) {
  return {
    (std::is_same_v<Policies, Policy> ?  policy : 
          static_cast<Policies>(policy_aggr))...
  };
}

template <Policy Policy, class... Policies>
auto operator|(Policy policy, policy_aggregate<Policies...> policy_aggr) {
  return (policy | ... | static_cast<Policies>(policy_aggr));
}
} // namespace satyr
