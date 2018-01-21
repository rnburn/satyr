load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "log_context",
    hdrs = glob(["include/satyr/**/*.h"]),
    srcs = glob(["src/*.h", "src/*.cpp"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"]
)
