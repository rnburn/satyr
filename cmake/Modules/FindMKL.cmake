include(FindPackageHandleStandardArgs)

if ($ENV{MKLROOT})
  set(MKL_ROOT $ENV{MKLROOT} CACHE PATH "Folder contains MKL")
else()
  set(MKL_ROOT "/opt/intel/mkl" CACHE PATH "Folder contains MKL")
endif()

find_path(MKL_INCLUDE_DIR mkl.h PATHS ${MKL_ROOT}/include)

if (NOT MKL_LIBRARIES)
  find_library(MKL_LIBRARY mkl_intel PATHS ${MKL_ROOT}/lib)
  set(MKL_LIBRARIES ${MKL_LIBRARY})
endif()
