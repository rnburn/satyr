#include <satyr/linear_algebra.h>

int main() {
  satyr::matrix<double> m1(3, 3);
  satyr::symmetric_matrix<double> s1(3);
  satyr::lower_triangular_matrix<double> l1(3);
  satyr::upper_triangular_matrix<double> u1(3);

  l1 = 3;
  u1 = u1 + 1.0;
  s1 *= 5.0;

  auto expr1 = m1 + s1;
  auto expr2 = m1 + l1;
  auto expr3 = m1 + u1;
  auto expr4 = u1 + l1;

  m1 = s1;
  m1 = l1;
  m1 = u1;
  return 0;
}