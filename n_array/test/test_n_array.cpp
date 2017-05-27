#include <satyr/n_array.h>
#include <cassert>

int main() {
  satyr::n_array<double, 2, satyr::general_structure> array = {
      {1, 2, 3, 4, 5}, {5, 6, 7, 8, 9}, {10, 11, 12, 13, 14}};
  array(0, 1) = 2;
  satyr::n_array_cview<double, 2, satyr::general_structure> view = array;
  assert(view(0, 1) == 2);
  return 0;
}
