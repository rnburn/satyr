load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "cblas",
    hdrs = glob(['include/satyr/*.h']),
    strip_include_prefix = 'include',
    visibility = ["//visibility:public"]
)

