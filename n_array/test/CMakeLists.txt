add_executable(test_n_array test_n_array.cpp)
add_test(test_n_array test_n_array)
target_link_libraries(test_n_array ${TBB_LIB})

add_executable(test_map test_map.cpp)
add_test(test_map test_map)
target_link_libraries(test_map ${TBB_LIB})
