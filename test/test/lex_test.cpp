#include "../src/lex.h"
#include <cassert>

int main() {
  // Whitespace is ignored.
  {
    auto stream = satyr::testing::lex("   \t\n");
    assert(stream.empty());
  }

  // Comments are ignored.
  {
    auto stream = satyr::testing::lex("//cat\n");
    assert(stream.empty());
  }

  {
    auto stream = satyr::testing::lex("3.5 cat {");
    assert((stream.consume<double>() == 3.5));
    assert((stream.consume<std::string_view>() == "cat"));
    stream.consume<satyr::testing::l_bracket_token>();
    assert(stream.empty());
  }
  return 0;
}
