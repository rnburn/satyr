#include <satyr/linear_algebra.h>
#include <random>
using satyr::index_t;

static thread_local std::mt19937 rng{std::random_device{}()};

int main() {
  satyr::matrix<double> a(5, 5), b(5, 5);
  satyr::symmetric_matrix<double> c(5);

  // Randomly initialize matrices.
  std::uniform_real_distribution<double> dist{-10, 10};
  for_each(a, [&] (double& x) { x = dist(rng); });
  for_each(satyr::parallel_v, b, [&] (double& x) { x = dist(rng); });
  for_each(c, [&](double& x, index_t i, index_t j) {
    x = dist(rng) + (i == j) * dist(rng);
  });
  std::cout << "a = " << a << "\n";
  std::cout << "b = " << b << "\n";
  std::cout << "c = " << c << "\n";

  a += b + square(a);
  std::cout << "a = " << a << "\n";

  a = cos(b) - sin(a) << satyr::parallel_v << satyr::simd_v;
  std::cout << "a = " << a << "\n";

  c = sqrt(abs(c)) << satyr::parallel_v;
  std::cout << "c = " << c << "\n";

  a += b - c;
  std::cout << "a = " << a << "\n";

  // multi-dimensional arrays
  satyr::n_array<double, 3> a3(5, 2, 6);
  for_each(a3, [&](double& x) { return x = dist(rng); });
  std::cout << "a3 = " << a3 << "\n";
  a += a3(satyr::all_v, 1, satyr::range{1, 6});
  std::cout << "a = " << a << "\n";
  return 0;
}
