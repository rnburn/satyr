#include <iostream>
#include <satyr/execution_policy.h>
#include <cassert>
using namespace satyr;

int main() {
  auto p1 = simd_v | nontemporal_v;
  assert((has_policy_v<simd, decltype(p1)>));
  assert((has_policy_v<nontemporal, decltype(p1)>));

  auto p2 = simd_v | nosimd_v;
  assert(!(has_policy_v<simd, decltype(p2)>));
  assert((has_policy_v<nosimd, decltype(p2)>));

  auto p3 = simd_v | grainsize{10};
  auto g1 = get_policy<grainsize>(p3);
  assert(g1.value == 10);

  auto p4 = grainsize{10} | grainsize{15};
  auto g2 = get_policy<grainsize>(p4);
  assert(g2.value == 15);
  return 0;
}
