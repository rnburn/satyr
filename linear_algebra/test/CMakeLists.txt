add_executable(test_matrix test_matrix.cpp)
add_test(test_matrix test_matrix)
target_link_libraries(test_matrix ${TBB_LIB})
