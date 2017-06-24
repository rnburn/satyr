cmake_minimum_required(VERSION 3.0)
enable_testing()

project(satyr)

set(SATYR_MAJOR_VERSION "0")
set(SATYR_MINOR_VERSION "0")
set(SATYR_PATCH_VERSION "0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fconcepts -fopenmp-simd")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
option(SATYR_WITH_TBB "Use MKL as backend for BLAS and LAPACK calls" OFF)
option(SATYR_WITH_MKL "Use MKL as backend for BLAS and LAPACK calls" OFF)
option(SATYR_WITH_JEMALLOC "Use jemalloc for memory allocation." OFF)

configure_file(config.h.in include/satyr/config.h)
configure_file(version.h.in include/satyr/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/satyr
        DESTINATION include)

###############################################################################
# Configure TBB
###############################################################################
if (SATYR_WITH_TBB)
  find_package(TBB REQUIRED)
  include_directories(SYSTEM ${TBB_INCLUDE_DIRS})
  link_libraries(${TBB_LIBRARIES})
endif()

###############################################################################
# Configure MKL
###############################################################################
set(WITH_BLAS_LAPACK OFF)
if (SATYR_WITH_MKL)
  find_package(MKL REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  include_directories(${MKL_INCLUDE_DIR})
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${MKL_LIBRARIES}")
  set(WITH_BLAS_LAPACK ON)
endif()

###############################################################################
# Configure MKL
###############################################################################
if (SATYR_WITH_JEMALLOC)
  find_package(Jemalloc REQUIRED)
  include_directories(SYSTEM ${JEMALLOC_INCLUDE_DIR})
  link_libraries(${JEMALLOC_LIBRARY})
endif()

###############################################################################
# Set up satyr modules
###############################################################################
set(MODULE_LIST k_array n_array linear_algebra)

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
