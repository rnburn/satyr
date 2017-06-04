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
  assert(expected_result.shape() == result.shape());
  for (index_t i=0; i<m; ++i)
    assert(expected_result(i) == result(i));
}

template <Matrix A, Matrix B>
void test_product(const A& a, const B& b) {
  auto [m, k] = a.shape();
  auto n = get_extent<1>(b);
  auto expected_result = [&] {
    matrix<double> c(m, n);
    c = 0;
    for (index_t i=0; i<m; ++i)
      for (index_t j=0; j<n; ++j)
        for (index_t t=0; t<k; ++t)
          c(i, j) += a(i, t) * b(t, j);
    return c;
  }();
  auto result = product(a, b);
  assert(expected_result.shape() == result.shape());
  for (index_t i=0; i<m; ++i)
    for (index_t j=0; j<n; ++j)
      assert(expected_result(i, j) == result(i, j));
}

int main() {
  matrix<double> a = {{1, 2}, {3, 4}};
  matrix<double> b = {{8, 1}, {9, -1}};
  lower_triangular_matrix<double> l = {{1}, {3, -2}};
  symmetric_matrix<double> s = {{1}, {2, 1}};
  vector<double> v = {7, 8};
  // gemv
  test_product(a, v);

  // gemm
  test_product(a, b);

  // symv
  test_product(s, v);

  // symm
  test_product(s, a);
  test_product(a, s);

  // trmv
  test_product(l, v);

  // trmm
  test_product(l, a);
  test_product(a, l);

  return 0;
}
