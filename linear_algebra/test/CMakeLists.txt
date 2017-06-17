add_executable(matrix_test matrix_test.cpp)
add_test(matrix_test matrix_test)

if (WITH_BLAS_LAPACK)
  add_executable(blas_lapack_test blas_lapack_test.cpp)
  add_test(blas_lapack_test blas_lapack_test)
endif()
