#include <satyr/n_array.h>
#include <cassert>

int main() {
  satyr::n_array<double, 2, satyr::general_structure> array = {
      {1, 2, 3, 4, 5}, {5, 6, 7, 8, 9}, {10, 11, 12, 13, 14}};
  array(0, 1) = 2;
  satyr::n_array_cview<double, 2, satyr::general_structure> view = array;
  assert(view(0, 1) == 2);

  satyr::n_array<double, 3> array2 = {{{3, 4}, {7, 5}}, {{12, 9}, {-1, 3}}};
  satyr::n_array<double, 3> array3{array2};
  array3(0, 1, 0) += 1;
  assert(array2 == array2);
  assert(array2 != array3);
  return 0;
}
