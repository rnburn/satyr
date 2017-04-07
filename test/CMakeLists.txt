add_executable(test_concept test_concept.cpp)
add_test(test_concept test_concept)

add_executable(test_policy test_policy.cpp)
add_test(test_policy test_policy)

add_executable(test_for test_for.cpp)
target_link_libraries(test_for ${TBB_LIB})
add_test(test_for test_for)
