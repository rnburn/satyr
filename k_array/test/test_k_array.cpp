#include <satyr/k_array.h>
#include <cassert>

int main() {
  satyr::k_array<double, 2> array({3, 5});
  array(0, 0) = 5;
  assert(array(0, 0) == 5);
  return 0;
}
