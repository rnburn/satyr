cmake_minimum_required(VERSION 3.0)
enable_testing()

project(satyr)

# ==============================================================================
# Version information

set(SATYR_MAJOR_VERSION "0")
set(SATYR_MINOR_VERSION "0")
set(SATYR_PATCH_VERSION "0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fconcepts -fopenmp-simd")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
option(WITH_BLAS_LAPACK "Integrate with BLAS and LAPACK" OFF)
option(WITH_JEMALLOC "Use jemalloc for memory allocation." OFF)

# ==============================================================================
# Configure TBB

set(SATYR_WITH_TBB OFF)
if (WITH_TBB)
  set(SATYR_WITH_TBB ON)
endif()

# ==============================================================================
# Configure BLAS and LAPACK

set(SATYR_WITH_BLAS_LAPACK OFF)
if (WITH_BLAS_LAPACK)
  set(SATYR_WITH_BLAS_LAPACK ON)
  include_directories(SYSTEM 3rd_party/netlib/include)
  install(DIRECTORY 3rd_party/netlib/include/satyr/ DESTINATION include/satyr
    FILES_MATCHING PATTERN "*.h")
endif()

# ==============================================================================
# Configure jemalloc

set(SATYR_WITH_JEMALLOC OFF)
if (WITH_JEMALLOC)
  set(SATYR_WITH_JEMALLOC ON)
endif()

# ==============================================================================
# Set up config files

configure_file(config.h.in include/satyr/config.h)
configure_file(version.h.in include/satyr/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/satyr
        DESTINATION include)

# ==============================================================================
# Set up satyr modules

set(MODULE_LIST core k_array n_array linear_algebra)

foreach(MODULE ${MODULE_LIST})
  include_directories(${MODULE}/include)
  install(DIRECTORY ${MODULE}/include/satyr/ DESTINATION include/satyr
    FILES_MATCHING PATTERN "*.h")
  install(DIRECTORY ${MODULE}/include/satyr/ DESTINATION include/satyr
    FILES_MATCHING PATTERN "*.def")
endforeach()
