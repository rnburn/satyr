#include "../src/token_stream.h"
#include <cassert>

int main() {
  auto empty_stream = satyr::testing::token_stream{{}};
  try {
    empty_stream.expect<double>();
    assert(0);
  } catch (const std::invalid_argument&) {
  }

  {
    auto stream = satyr::testing::token_stream{{3.0, "cat"}};
    stream.expect<double>();
    assert((stream.consume<double>() == 3.0));
    assert((stream.peek<std::string_view>()));
    assert((stream.consume<std::string_view>() == "cat"));
  }
  return 0;
}
