#include <satyr/linear_algebra.h>
using namespace satyr;

int main() {
  satyr::vector<double> v1 = {1, 2, 3};
  satyr::vector<double> v2 = {4, 5};
  v1(satyr::range{1, 3}) = v2;

  matrix<double> m1 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
  symmetric_matrix<double> s1(3);
  lower_triangular_matrix<double> l1(3);
  upper_triangular_matrix<double> u1(3);

  assert(norm1(m1) == 15);
  assert(norm2_squared(m1(0, satyr::all_v)) == 5);
  assert((get_diagonal(m1) == vector<double>{0, 4, 8}));
  l1 = 3;
  u1 = u1 + 1.0;
  s1 *= 5.0;

  [[maybe_unused]] auto expr1 = m1 + s1;
  [[maybe_unused]] auto expr2 = m1 + l1;
  [[maybe_unused]] auto expr3 = m1 + u1;
  [[maybe_unused]] auto expr4 = u1 + l1;

  m1 = s1;
  m1 = l1;
  m1 = u1;

  m1 = m1 + as_diagonal_matrix(get_diagonal(m1));
  m1 = m1 + identity_matrix(3) * 5;

  symmetric_matrix<double> s2 = {{1, 2, 3}, {2, 4, 5}, {3, 5, 6}};
  assert(sum_elements(s2) == 31);
  return 0;
}
