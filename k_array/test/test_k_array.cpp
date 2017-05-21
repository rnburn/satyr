#include <satyr/k_array.h>
#include <cassert>
#include <iostream>

int main() {
  satyr::k_array<double, 2> array({3, 2});
  array(0, 0) = 5;
  array(1, 0) = 6;
  array(2, 0) = 7;
  array(0, 1) = 8;
  array(1, 1) = 9;
  array(2, 1) = 10;
  assert(array(0, 0) == 5);

  auto subview1 = array(satyr::all_v, 0);
  assert(subview1(0) == 5);
  assert(subview1(1) == 6);
  assert(subview1(2) == 7);

  auto subview2 = array(0, satyr::all_v);
  assert(subview2(0) == 5);
  assert(subview2(1) == 8);

  auto subview3 = array(satyr::range{1, 3}, 1);
  assert(subview3(0) == 9);
  assert(subview3(1) == 10);
  return 0;
}
