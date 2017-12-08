load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "n_array",
    hdrs = glob(["include/satyr/**/*.h"]),
    strip_include_prefix = "include",
    deps = ["//k_array:k_array"],
    visibility = ["//visibility:public"]
)

