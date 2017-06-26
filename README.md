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
* [TBB](https://www.threadingbuildingblocks.org/) for parallelization. (optional but recommended)
* [MKL](https://software.intel.com/en-us/mkl) for BLAS/LAPACK functionality. (optional)
* [jemalloc](http://jemalloc.net/). (optional).

### Installation
```
git clone https://github.com/rnburn/satyr
cd satyr
mkdir .build && cd .build
cmake -DSATYR_WITH_TBB=[ON|OFF] -DSATYR_WITH_MKL=[ON|OFF] -DSATYR_WITH_JEMALLOC=[ON|OFF] ..
make && make install
```
