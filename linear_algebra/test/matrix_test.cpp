#include <satyr/linear_algebra.h>
using namespace satyr;

int main() {
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

  return 0;
}
