#pragma once

#include <satyr/execution_policy.h>
#include <satyr/concept.h>
#include <satyr/n_array/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// scalar_expression
//------------------------------------------------------------------------------
template <Scalar Scalar, Policy Policy>
class scalar_expression {
 public:
  using value_type = Scalar;

  scalar_expression(Scalar value, Policy policy)
      : value_{value}, policy_{policy} {}

  Scalar value() const { return value_; }

  Policy policy() const { return policy_; }

 private:
  Scalar value_;
  Policy policy_;
};
} // namespace satyr
