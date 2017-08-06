#include <satyr/for.h>
#include <cassert>
using namespace satyr;

void test_for_each_index() {
  const index_t m = 10, n = 20;
  index_t a[m][n] = {};
  for_each_index(grainsize{5}, std::array<index_t, 2>{m, n},
                 [&](index_t i, index_t j) { a[i][j] = i + m * j; });
  for (index_t i=0; i<m; ++i)
    for (index_t j=0; j<n; ++j)
      assert(a[i][j] == i+m*j);
}

int main() {
  return 0;
}
