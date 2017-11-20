#include "../src/array.h"
#include "../src/lex.h"
#include <cassert>

int main() {
  {
    auto stream = satyr::testing::lex("{}");
    auto [values, dimensions] = parse_array(1, stream);
    assert(values.empty());
    assert(dimensions.size() == 1);
    assert(dimensions.at(0) == 0);
  }

  {
    auto stream = satyr::testing::lex("{1, 2}");
    auto [values, dimensions] = parse_array(1, stream);
    assert((values == std::vector<double>{1, 2}));
    assert(dimensions.size() == 1);
    assert(dimensions.at(0) == 2);
  }

  {
    auto stream = satyr::testing::lex("{{1, 2}, {3, 4}, {5, 6}}");
    auto [values, dimensions] = parse_array(2, stream);
    assert((values == std::vector<double>{1, 2, 3, 4, 5, 6}));
    assert(dimensions.size() == 2);
    assert(dimensions.at(0) == 3);
    assert(dimensions.at(1) == 2);
  }
  return 0;
}
