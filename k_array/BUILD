load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "k_array",
    hdrs = glob(["include/satyr/**/*.h"]),
    strip_include_prefix = "include",
    deps = ["//core:core"],
    visibility = ["//visibility:public"]
)

