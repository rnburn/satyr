#pragma once

#include <satyr/concept.h>

namespace satyr {
//------------------------------------------------------------------------------
// scalar_evaluator
//------------------------------------------------------------------------------
template <Scalar T>
class scalar_evaluator {
 public:
   explicit scalar_evaluator(T value) : value_{value} {}

   T operator()(index_t) const { return value_; }
 private:
   T value_;
};
} // namespace satyr
