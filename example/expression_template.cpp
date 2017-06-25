#include <satyr/linear_algebra.h>
#include <random>
using satyr::index_t;

static thread_local std::mt19937 rng{std::random_device{}()};

int main() {
  satyr::matrix<double> a(5,5), b(5,5);
  satyr::symmetric_matrix<double> c(5);
  std::uniform_real_distribution<double> dist{-10, 10};
  for_each(a, [&] (double& x) { x = dist(rng); });
  for_each(satyr::parallel_v, b, [&] (double& x) { x = dist(rng); });
  // TODO: fix-me
  for_each(c, [&] (double& x, index_t i, index_t j) { x = dist(rng) + (i == j)*dist(rng); });
  std::cout << "a = " << a << "\n";
  std::cout << "b = " << b << "\n";
  std::cout << "c = " << c << "\n";

  a += b + square(a);
  std::cout << "a = " << a << "\n";

  // TODO: fix-me
  a = cos(b) - sin(a) << satyr::parallel_v; // | satyr::simd_v;
  std::cout << "a = " << a << "\n";
  return 0;
}
