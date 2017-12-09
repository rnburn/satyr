load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "linear_algebra",
    hdrs = glob(["include/satyr/**/*.h"]),
    strip_include_prefix = "include",
    deps = ["//n_array:n_array"],
    visibility = ["//visibility:public"]
)
