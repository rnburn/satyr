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
    deps = [
        "//3rd_party/netlib:cblas", 
        "@org_threadingbuildingblocks//:tbb",
        "@org_netlib_blas//:cblas",
        "@org_netlib_lapack//:lapacke",
    ],
    visibility = ["//visibility:public"]
)

genrule(
    name = "with_tbb_h",
    outs = ["with_tbb.h"],
    cmd = select({
        "//bazel:with_tbb": """echo "#define SATYR_WITH_TBB" > $@""",
        "//conditions:default": """touch $@""",
    })
)

genrule(
    name = "with_blas_lapack_h",
    outs = ["with_blas_lapack.h"],
    cmd = select({
        "//bazel:with_blas_lapack": """echo "#define SATYR_WITH_BLAS_LAPACK" > $@""",
        "//conditions:default": """touch $@""",
    })
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
