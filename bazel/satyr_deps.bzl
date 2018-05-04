def satyr_deps():
  if "com_github_01org_tbb" not in native.existing_rules():
    native.new_http_archive(
      name = "com_github_01org_tbb",
      build_file = "//bazel:tbb.BUILD",
      url = "https://github.com/01org/tbb/archive/2018_U3.tar.gz",
      strip_prefix = "tbb-2018_U3",
    )

  if "org_netlib" not in native.existing_rules(): 
    native.new_http_archive(
      name = "org_netlib_src",
      build_file = "//bazel:netlib_src.BUILD",
      url = "http://www.netlib.org/lapack/lapack-3.8.0.tar.gz",
      strip_prefix = "lapack-3.8.0",
    )

    native.new_http_archive(
      name = "org_netlib",
      build_file = "//bazel:netlib.build",
      url = "https://github.com/xianyi/openblas/archive/v0.2.20.tar.gz",
      strip_prefix = "OpenBLAS-0.2.20",
    )
