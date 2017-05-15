#include <satyr/linear_algebra.h>
#include <random>
#include <cmath>
#include <iostream>
#include <chrono>
#include <tbb/task_scheduler_init.h>
using satyr::index_t;
using std::chrono::steady_clock;

std::unique_ptr<double[]> generate_matrix(int m, int n) {
  std::unique_ptr<double[]> data{new double[m * n]};
  std::mt19937 rng{std::random_device{}()};
  std::uniform_real_distribution<double> dist{0, 1};
  for (int i=0; i<m*n; ++i)
    data[i] = dist(rng);
  return data;
}

void compute1(double* x, int m, int n) {
  int i;
  int size = m*n;
#pragma omp simd linear(i)
  for (i=0; i<size; ++i)
    x[i] = std::sqrt(x[i]);
}

void compute2(double* x, int m, int n) {
  auto matrix = satyr::matrix_view<double>{x, satyr::shape<2>{m, n}};
  matrix = sqrt(matrix);
}

void compute3(double* x, int m, int n) {
  auto matrix = satyr::matrix_view<double>{x, satyr::shape<2>{m, n}};
  matrix = sqrt(matrix) << satyr::parallel_v;
}

template <class T>
void do_not_optimize_away(T&& datum) {
  asm volatile("" : "+r" (datum));
}

void benchmark(int i, int m, int n) {
  auto matrix = generate_matrix(m, n);
  auto t0 = steady_clock::now();
  switch(i) {
    case 0:
      compute1(matrix.get(), m, n);
      break;
    case 1:
      compute2(matrix.get(), m, n);
      break;
    case 2:
      compute3(matrix.get(), m, n);
      break;
  }
  auto t1 = steady_clock::now();
  do_not_optimize_away(matrix.get());
  std::cout
      << i << ": "
      << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count()
      << "\n";
}

int main() {
  tbb::task_scheduler_init init;
  int m = 200, n = 200;
  for (int i = 0; i < 5; ++i) {
    benchmark(0, m, n);
    benchmark(1, m, n);
    benchmark(2, m, n);
  }
  return 0;
}
