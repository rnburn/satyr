#include <satyr/linear_algebra.h>

int main() {
  satyr::matrix<double> m1(3, 3);
  satyr::symmetric_matrix<double> s1(3);
  satyr::lower_triangular_matrix<double> l1(3);
  satyr::upper_triangular_matrix<double> u1(3);

  l1 = 3;
  u1 = u1 + 1.0;
  s1 *= 5.0;
  return 0;
}
