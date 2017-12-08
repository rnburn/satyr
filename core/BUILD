load(
    "//bazel:satyr_build_system.bzl",
    "satyr_cc_library"
)

satyr_cc_library(
    name = "core",
    hdrs = glob([
        'include/satyr/*.h',
        'include/satyr/*.def',
        ]) + [":include/satyr/config.h"],
    strip_include_prefix = 'include',
    deps = ["//3rd_party/netlib:cblas"],
    visibility = ["//visibility:public"]
)

genrule(
    name = "generate_config_h",
    outs = [
        "include/satyr/config.h"
    ],
    cmd = """
    cat <<END  > $${PWD}/$(location :include/satyr/config.h)
#pragma once

#define SAYR_WITH_TBB
#define SATYR_WITH_BLAS_LAPACK
END
""",
)
