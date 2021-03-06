#include <satyr/k_array.h>
#include <cassert>
#include <iostream>
#include <sstream>

int main() {
  satyr::k_array<int, 2> array = {{5, 8}, {6, 9}, {7, 10}};
  assert(array(0, 0) == 5);
  auto [m, n] = array.shape();
  assert(m == 3);
  assert(n == 2);
  assert(m == array.extent(0));
  assert(n == array.extent(1));

  std::ostringstream oss1;
  oss1 << array;
  assert(oss1.str() == "{{5, 8}, {6, 9}, {7, 10}}");

  auto subview1 = array(satyr::all_v, 0);
  assert(subview1.extent(0) == array.extent(0));
  assert(subview1(0) == 5);
  assert(subview1(1) == 6);
  assert(subview1(2) == 7);

  std::ostringstream oss2;
  oss2 << subview1;
  assert(oss2.str() == "{5, 6, 7}");

  auto subview2 = array(0, satyr::all_v);
  assert(subview2(0) == 5);
  assert(subview2(1) == 8);

  auto subview3 = array(satyr::range{1, 3}, 1);
  assert(subview3(0) == 9);
  assert(subview3(1) == 10);

  auto subview4 = array(satyr::range{1, 3}, satyr::range{0,1});
  assert(subview4(0, 0) == 6);

  auto subview5 = subview4(satyr::range{1, 2}, satyr::range{0,1});
  assert(subview5(0, 0) == 7);

  // shape deduction guide
  assert((satyr::shape(1, 2) == satyr::shape<2>{1,2}));
  return 0;
}
