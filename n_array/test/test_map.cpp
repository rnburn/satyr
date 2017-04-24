#include <satyr/n_array.h>
#include <cassert>
using namespace satyr;

int main() {
  n_array<double, 2, satyr::general_structure> array1(3, 5), array2(3,5);
  n_array<double, 3, satyr::general_structure> array3(3, 5, 6);
  auto expr1 = make_expression(array1);
  auto expr2 = make_expression(expr1);
  auto expr3 = make_expression(2.0);
  auto f = [](double x, double y) { return x + y; };
  auto eval = expr1.evaluator();
  auto expr4 = map(f, array1, array2);
  auto expr5 = map(f, array1, 5.0);

  // auto expr6 = map(f, array1, array3); // won't compile

  auto expr7 = array1 + array2;
  return 0;
}
