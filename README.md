satyr
-----
A library for numerical computing. It provides
* Expression templates for vectors, matrices, and higher-dimensional arrays.
* Support for execution policies including SIMD vectorization and task-based
parallelization.
* Strongly typed versions of structural matrices (e.g. triangular and
symmetric).
* A modern C++ interface to BLAS and LAPACK.

### Dependencies
* [GCC 7](https://gcc.gnu.org/) or later.
* [TBB](https://www.threadingbuildingblocks.org/) for parallelization.
  (optional but recommended)
* BLAS and LAPACK libraries (See, for example,
  [MKL](https://software.intel.com/en-us/mkl) or
  [ATLAS](http://math-atlas.sourceforge.net/) (optional).
* [jemalloc](http://jemalloc.net/). (optional).

### Installation
```
git clone https://github.com/rnburn/satyr
cd satyr
mkdir .build && cd .build
cmake -DWITH_TBB=[ON|OFF] \
      -DWITH_JEMALLOC=[ON|OFF]  \
      # Note: If WITH_BLAS_LAPACK is set CBLAS and LAPACKE libraries must also
      # be linked in via CMAKE_EXE_LINKER_FLAGS.
      -DWITH_BLAS_LAPACK=[ON|OFF] -DCMAKE_EXE_LINKER_FLAGS="<blas-lapack-libraries>" \
      ..
make && make install
```

### Quick Start
```cpp
// Declare some random vector and matrices.
satyr::vector<double> v(5);
satyr::matrix<double> A(5, 5), B(5, 5);
satyr::symmetric_matrix<double> S(5);
satyr::lower_triangular_matrix<double> L(5);

// The standard arithmetic operators and mathematical functions can be used to
// execute expression templates.
A = B + 2.0 * cos(S);

// Expressions involving only structural matrices are computed in an efficient
// manner that avoids unnecessary work.
S = sqrt(abs(S));  // computes only over a triangular portion of the matrix.

// Additionally execution policies can be applied to parallelize or vectorize
// the computation.
A = sqrt(abs(L)) << satyr::parallel_v << satyr::simd_v;

// For parallelization, you can also specify a grainsize if the cost of
// managing tasks could potentially be more expensive than the computation
// itself.
A += cos(B) - as_diagonal_matrix(v) << satyr::grainsize{
         10};  // Don't create tasks with fewer than 10 iterations.

// Addtionality you can declare numerical arrays of arbitrary dimension.
satyr::n_array<float, 3> H(5, 2, 6);

// And all array-like objects support indexing and slicing.
A(0, 0) -= 5;
A += H(satyr::all_v, 1, satyr::range{1, 6});

// There is a wrapper for many BLAS-LAPACK functions.
auto C = product(A, B);     // calls gemm.
auto w = left_solve(L, v);  // calls trsv.
```
