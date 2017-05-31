add_executable(test_matrix test_matrix.cpp)
add_test(test_matrix test_matrix)

if (WITH_BLAS_LAPACK)
  add_executable(test_blas_lapack test_blas_lapack.cpp)
  add_test(test_blas_lapack test_blas_lapack)
endif()
