include(FindPackageHandleStandardArgs)

if ($ENV{MKLROOT})
  set(MKL_ROOT $ENV{MKLROOT} CACHE PATH "Folder contains MKL")
else()
  set(MKL_ROOT "/opt/intel/mkl" CACHE PATH "Folder contains MKL")
endif()

find_path(MKL_INCLUDE_DIR mkl.h PATHS ${MKL_ROOT}/include)

if (NOT MKL_LIBRARIES)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMKL_ILP64 -m64")
  set(CMAKE_SHARED_LINKER_FLAGS 
    "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,${MKL_ROOT}/lib")
  find_library(MKL_LIBRARY mkl_intel_ilp64 PATHS ${MKL_ROOT}/lib)
  find_library(MKL_SEQUENTIAL_LIBRARY mkl_sequential PATHS ${MKL_ROOT}/lib)
  find_library(MKL_CORE_LIBRARY mkl_core PATHS ${MKL_ROOT}/lib)
  set(MKL_LIBRARIES
    "${MKL_LIBRARY} ${MKL_SEQUENTIAL_LIBRARY} ${MKL_CORE_LIBRARY}")
endif()
