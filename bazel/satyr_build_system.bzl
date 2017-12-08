def satyr_copts():
  return [
      "-std=c++1z",
      "-fconcepts",
      "-fopenmp-simd",
  ]

def satyr_cc_library(name,
                     srcs = [],
                     hdrs = [],
                     copts = [],
                     visibility = None,
                     deps = [],
                     strip_include_prefix = None):
  native.cc_library(
      name = name,
      srcs = srcs,
      hdrs = hdrs,
      copts = satyr_copts() + copts,
      deps = deps,
      visibility = visibility,
      strip_include_prefix = strip_include_prefix,
  )

def satyr_cc_test(name,
                  srcs = [],
                  copts = [],
                  deps = [],
                  args = []):
  native.cc_test(
      name = name,
      srcs = srcs,
      copts = satyr_copts() + copts,
      deps = deps,
      args = args,
  )
