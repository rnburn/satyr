#include <satyr/linear_algebra.h>
#include <random>
using satyr::index_t;

int main() {
  // Declare some random vector and matrices.
  satyr::vector<double> v(5);
  satyr::matrix<double> A(5, 5), B(5, 5);
  satyr::symmetric_matrix<double> S(5);
  satyr::lower_triangular_matrix<double> L(5);

  // Randomly initialize matrices.
  std::mt19937 rng{0};
  std::uniform_real_distribution<double> dist{-10, 10};
  for_each(v, [&](double& element) { element = dist(rng); });
  for_each(A, [&](double& element) { element = dist(rng); });
  for_each(B, [&](double& element) { element = dist(rng); });
  for_each(S, [&](double& element) { element = dist(rng); });
  for_each(L, [&](double& element) { element = dist(rng); });
  std::cout << "v = " << v << "\n";
  std::cout << "A = " << A << "\n";
  std::cout << "B = " << B << "\n";
  std::cout << "S = " << S << "\n";
  std::cout << "L = " << L << "\n";

  // The standard arithmetic operators and mathematical functions can be used to
  // execute expression templates.
  std::cout << "A = B + 2.0 * cos(S)\n";
  A = B + 2.0 * cos(S);
  std::cout << "A = " << A << "\n";

  // Expressions involving only structural matrices are computed in an efficient
  // manner that avoids unnecessary work.
  std::cout << "S = sqrt(abs(S))\n";
  S = sqrt(abs(S));  // computes only over a triangular portion of the matrix.
  std::cout << "S = " << S << "\n";

  // Additionally execution policies can be applied to parallelize or vectorize
  // the computation.
  std::cout << "A = sqrt(abs(L))\n";
  A = sqrt(abs(L)) << satyr::parallel_v << satyr::simd_v;
  std::cout << "A = " << A << "\n";

  // For parallelization, you can also specify a grainsize if the cost of
  // managing tasks could potentially be more expensive than the computation
  // itself.
  std::cout << "A += cos(B) - as_diagonal_matrix(v)\n";
  A += cos(B) - as_diagonal_matrix(v) << satyr::grainsize{
           10};  // Don't create tasks with fewer than 10 iterations.
  std::cout << "A = " << A << "\n";

  // Addtionality you can declare numerical arrays of arbitrary dimension.
  satyr::n_array<float, 3> H(5, 2, 6);
  for_each(H, [&](float& element) { element = static_cast<float>(dist(rng)); });
  std::cout << "H = " << H << "\n";

  // And all array-like objects support indexing and slicing.
  std::cout << "A(0, 0) -= 5\n";
  std::cout << "A += H(satyr_v::all_v, 1, satyr::range{1,6})\n";
  A(0, 0) -= 5;
  A += H(satyr::all_v, 1, satyr::range{1, 6});
  std::cout << "A = " << A << "\n";

  // There is a wrapper for many BLAS-LAPACK functions.
  std::cout << "C = product(A, B)\n";
  std::cout << "w = left_solve(L, v)\n";
  auto C = product(A, B);     // calls gemm.
  auto w = left_solve(L, v);  // calls trsv.
  std::cout << "C = " << C << "\n";
  std::cout << "w = " << w << "\n";
  return 0;
}
