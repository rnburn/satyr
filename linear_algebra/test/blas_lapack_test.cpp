#include <satyr/linear_algebra.h>
#include <cassert>
using namespace satyr;

template <OperationMatrix A, Vector X>
void test_product(const A& a, const X& x) {
  auto [m, n] = a.shape();
  assert(n == get_extent<0>(x));
  auto expected_result = [&]{
    vector<double> y(m);
    y = 0;
    for (index_t i=0; i<m; i++)
      for (index_t j=0; j<n; j++)
        y(i) += a(i, j)*x(j);
    return y;
  }();
  auto result = product(a, x);
  assert(expected_result == result);
}

template <OperationMatrix A, OperationMatrix B>
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
  auto sa = a(range{0, 1}, all_v);
  auto sv = a(1, all_v);
  matrix<double> b = {{8, 1}, {9, -1}};
  lower_triangular_matrix<double> l = {{1}, {3, -2}};
  symmetric_matrix<double> s = {{1}, {2, 1}};
  symmetric_matrix<double> i = {{1}, {0, 1}};
  vector<double> v = {7, 8};

  // gemv
  test_product(a, v);
  test_product(sa, sv);
  test_product(transpose(a), v);

  // gemm
  test_product(a, b);
  test_product(transpose(a), b);
  test_product(a, transpose(b));

  // symv
  test_product(s, v);

  // symm
  test_product(s, a);
  test_product(a, s);

  // trsv
  left_solve(l, v);
  left_solve(transpose(l), v);
  left_solve(l, a);
  left_solve(transpose(l), a);

  // trmv
  test_product(l, v);

  // trmm
  test_product(l, a);
  test_product(a, l);

  // potrf
  assert(cholesky_factorize(i));
  cholesky_invert(l);
  auto [factorization, remainder] = inplace_cholesky_factorize_until_failure(i);
  return 0;
}
