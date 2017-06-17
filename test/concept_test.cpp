#include <satyr/concept.h>
#include <cassert>
using namespace satyr;

int main() {
  auto f1 = [](index_t) {};
  assert((IndexFunctor<decltype(f1), 1>));
  return 0;
}
