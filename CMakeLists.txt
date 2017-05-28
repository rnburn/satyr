cmake_minimum_required(VERSION 3.0)
enable_testing()

project(satyr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fconcepts -fopenmp-simd")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
option(SATYR_WITH_MKL "Use MKL as backend for BLAS and LAPACK calls" OFF)

find_path(TBB_INCLUDE_DIR NAMES tbb/parallel_for.h)
find_library(TBB_LIB tbb)

if (SATYR_WITH_MKL)
  find_package(MKL REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSATYR_WITH_MKL")
  include_directories(${MKL_INCLUDE_DIR})
endif()

include_directories(${TBB_INCLUDE_DIR})
set(MODULE_LIST
k_array
n_array
linear_algebra
)


include_directories(include)
install(DIRECTORY include/satyr/ DESTINATION include/satyr
  FILES_MATCHING PATTERN "*.h")
add_subdirectory(test)

foreach(MODULE ${MODULE_LIST})
  include_directories(${MODULE}/include)
  install(DIRECTORY ${MODULE}/include/satyr/ DESTINATION include/satyr
    FILES_MATCHING PATTERN "*.h")
  install(DIRECTORY ${MODULE}/include/satyr/ DESTINATION include/satyr
    FILES_MATCHING PATTERN "*.def")
endforeach()

foreach(MODULE ${MODULE_LIST})
  add_subdirectory(${MODULE})
endforeach()
