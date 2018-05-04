cc_library(
  name = "blas",
  hdrs = [
    "include/x86_64-linux-gnu/cblas.h",
  ],
  strip_include_prefix = "include/x86_64-linux-gnu",
  include_prefix = "netlib",
  srcs = [
    "lib/x86_64-linux-gnu/libcblas.a",
    "lib/x86_64-linux-gnu/libatlas.a",
  ],
  linkopts = ["-lgfortran", "-lm"],
  visibility = ["//visibility:public"]
)

cc_library(
  name = "lapack",
  srcs = [
    "lib/x86_64-linux-gnu/liblapacke.a",
    "lib/x86_64-linux-gnu/liblapack.a",
  ],
  deps = [":blas"],
  visibility = ["//visibility:public"],
)
