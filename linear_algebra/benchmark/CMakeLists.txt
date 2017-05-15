add_executable(bench_matrix bench_matrix.cpp)
add_test(bench_matrix bench_matrix)
target_link_libraries(bench_matrix ${TBB_LIB})
target_compile_options(bench_matrix PRIVATE -O3 -march=native)
