load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_test"
)

satyr_cc_test(
    name = "matrix_test",
    srcs = ["matrix_test.cpp"],
    deps = ["//linear_algebra:linear_algebra"],
)

satyr_cc_test(
    name = "blas_lapack_test",
    srcs = ["blas_lapack_test.cpp"],
    deps = ["//linear_algebra:linear_algebra"],
)
