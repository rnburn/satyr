#include <satyr/n_array.h>
#include <cassert>

int main() {
  satyr::n_array<double, 2, satyr::general_structure> array = {
      {1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
  array(0, 1) = 2;
  satyr::n_array_cview<double, 2, satyr::general_structure> view = array;
  assert(view(0, 1) == 2);

  satyr::n_array<double, 3> array2 = {{{3, 4}, {7, 5}}, {{12, 9}, {-1, 3}}};
  satyr::n_array<double, 3> array3{array2};
  array3(0, 1, 0) += 1;
  assert(array2 == array2);
  assert(array2 != array3);

  satyr::n_array<double, 2> array4{array(satyr::all_v, satyr::range{1, 3})};
  assert(array4 == array(satyr::all_v, satyr::range{1,3}));

  double array_sum = 120;
  double array_sum1 = 0;
  satyr::for_each(array, [&] (double x) {
      array_sum1 +=x;
      });
  assert(array_sum1 == array_sum);
  double array_sum2 = 0;
  satyr::for_each(array, [&] (double x, satyr::index_t i, satyr::index_t j) {
      array_sum2 += x + array(i, j);
  });
  assert(array_sum2 == 2*array_sum);
  return 0;
}
