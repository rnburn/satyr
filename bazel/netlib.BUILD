genrule(
	name = "build_openblas",
	srcs = glob(["**"]) + [
		"@local_config_cc//:toolchain",
    "@org_netlib_src//:lapack",
  ],
	cmd = """
				 set -e
				 WORK_DIR=$$PWD
				 DEST_DIR=$$PWD/$(@D)
         export PATH=$$(dirname $(AR)):$$PATH
				 export CC=$$PWD/$(CC)
         export CXX=$$PWD/$(CC)
				 export CXXFLAGS=$(CC_FLAGS)
			   export NM=$(NM)
				 export AR=$(AR)
         NETLIB_README=$$(echo $(locations @org_netlib_src//:lapack) | grep -o "[^ ]*README.md")
         export NETLIB_LAPACK_DIR=$$PWD/$$(dirname $$NETLIB_README)
			   cd $$(dirname $(location :Makefile))

         make

         export TOPDIR=$$PWD
         mkdir build
         export PREFIX=$$PWD/build
         make install
         cp build/lib/*.a $$DEST_DIR
         cp -r build/include $$DEST_DIR

	""",
	outs = [
    "include/cblas.h",
    "include/openblas_config.h",
    "include/lapacke.h",
    "include/lapacke_mangling.h",
    "include/lapacke_config.h",
		"libopenblas.a",
	]	
)

cc_library(
    name = "blas",
		srcs = ["libopenblas.a"],
    hdrs = [
      "include/cblas.h",
      "include/openblas_config.h",
    ],
    strip_include_prefix = "include",
    include_prefix = "netlib",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "lapack",
    hdrs = [
      "include/lapacke.h",
      "include/lapacke_mangling.h",
      "include/lapacke_config.h",
    ],
    deps = [
      ":blas",
    ],
    strip_include_prefix = "include",
    include_prefix = "netlib",
    visibility = ["//visibility:public"],
)
