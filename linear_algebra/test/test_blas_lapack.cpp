#include <satyr/linear_algebra.h>
#include <cassert>
using namespace satyr;

template <Matrix A, Vector X>
void test_product(const A& a, const X& x) {
  auto [m, n] = a.shape();
  assert(m == get_extent<0>(x));
  auto expected_result = [&]{
    vector<double> y(m);
    y = 0;
    for (index_t i=0; i<m; i++)
      for (index_t j=0; j<n; j++)
        y(i) += a(i, j)*x(j);
    return y;
  }();
  auto result = product(a, x);
  for (index_t i=0; i<m; ++i)
    assert(expected_result(i) == result(i));
}

int main() {
  matrix<double> a = {{1, 2}, {3, 4}};
  vector<double> v = {7, 8};
  test_product(a, v);
  return 0;
}
