#include <satyr/n_array.h>
#include <cassert>
using namespace satyr;

int main() {
  n_array<double, 2, general_structure> array = {
      {1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
  array(0, 1) = 2;
  n_array_cview<double, 2, general_structure> view = array;
  assert(array.extent(0) == view.extent(0));
  assert(array.extent(1) == view.extent(1));
  assert(view(0, 1) == 2);

  n_array<double, 3> array2 = {{{3, 4}, {7, 5}}, {{12, 9}, {-1, 3}}};
  n_array<double, 3> array3{array2};
  array3(0, 1, 0) += 1;
  assert(array2 == array2);
  assert(array2 != array3);

  n_array<double, 2> array4{array(all_v, range{1, 3})};
  assert(array4 == array(all_v, range{1,3}));
  array4 = 0;
  array4 = array(all_v, range{0, 2});
  assert(array4 == array(all_v, range{0, 2}));

  double array_sum = 120;
  double array_sum1 = 0;
  for_each(array, [&] (double x) {
      array_sum1 +=x;
      });
  assert(array_sum1 == array_sum);
  double array_sum2 = 0;
  for_each(array, [&] (double x, index_t i, index_t j) {
      array_sum2 += x + array(i, j);
  });
  assert(array_sum2 == 2*array_sum);

  assert(sum_elements(array) == array_sum);
  assert(max_element(array) == 15);
  assert(min_element(array) == 1);
  assert(min_element(array(all_v, range{1,3})) == 2);
  return 0;
}
