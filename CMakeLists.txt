cmake_minimum_required(VERSION 3.0)
enable_testing()

project(satyr)

find_library(TBB_LIB tbb)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fconcepts")

set(MODULE_LIST
  "\
k_array")


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
