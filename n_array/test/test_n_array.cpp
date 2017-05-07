#include <satyr/n_array.h>
#include <cassert>

int main() {
  satyr::n_array<double, 2, satyr::general_structure> array(3, 5);
  array(0, 1) = 22;
  satyr::n_array_cview<double, 2, satyr::general_structure> view = array;
  assert(view(0, 1) == 22);
  return 0;
}
