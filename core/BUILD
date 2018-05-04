load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "core",
    hdrs = glob([
        "include/satyr/*.h",
        "include/satyr/*.def",
        ]) + [":include/satyr/config.h"],
    strip_include_prefix = "include",
    deps =  [
        "@com_github_01org_tbb//:tbb",
        "@org_netlib//:blas",
        "@org_netlib//:lapack",
    ],
    visibility = ["//visibility:public"]
)

genrule(
    name = "with_tbb_h",
    outs = ["with_tbb.h"],
    cmd = """echo "#define SATYR_WITH_TBB" > $@""",
)

genrule(
    name = "with_blas_lapack_h",
    outs = ["with_blas_lapack.h"],
    cmd = """
      echo "#define SATYR_WITH_BLAS_LAPACK" > $@
      echo "#define SATYR_EXTERNAL_BLAS_LAPACK" >> $@
     """,
)

genrule(
    name = "generate_config_h",
    srcs = [
      ":with_tbb.h",
      ":with_blas_lapack.h"
    ],
    outs = [
        "include/satyr/config.h"
    ],
    cmd = """
    echo "#pragma once\n" > $@
    cat $(SRCS) >> $@
    """
)
