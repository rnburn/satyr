#include "lex.h"

#include <algorithm>

namespace satyr::testing {
//------------------------------------------------------------------------------
// starts_with
//------------------------------------------------------------------------------
static bool starts_with(std::string_view s1, std::string_view s2) {
  return s1.compare(0, s2.size(), s2) == 0;
}

//------------------------------------------------------------------------------
// lex
//------------------------------------------------------------------------------
token_stream lex(std::string_view s) {
  auto i = s.data();
  auto last = i + s.size();
  std::vector<token> tokens;
  while (1) {
    // Skip over whitespace
    i = std::find_if_not(i, last, [](char c) { return std::isspace(c); });

    // Check if we've reached the end.
    if (i == last) return token_stream{std::move(tokens)};

    auto s = std::string_view(&*i, std::distance(i, last));
    if (starts_with(s, "//---")) {
      tokens.push_back(divider_token{});
      i = std::find(i, last, '\n');
    } else if (starts_with(s, "//")) {
      i = std::find(i, last, '\n');
      continue;
    } else if (*i == '{') {
      tokens.push_back(l_bracket_token{});
      ++i;
    } else if (*i == '}') {
      tokens.push_back(r_bracket_token{});
      ++i;
    } else if (*i == ',') {
      tokens.push_back(comma_token{});
      ++i;
    } else if (*i == ';') {
      tokens.push_back(semicolon_token{});
      ++i;
    } else if (std::isalpha(*i)) {
      auto id_first = i;
      i = std::find_if_not(i, last, [](char c) { return std::isalnum(c); });
      auto id = std::string_view(&*id_first, std::distance(id_first, i));
      tokens.push_back(id);
    } else if (std::isdigit(*i) || *i == '-' || *i == '+') {
      char* value_last;
      double value = std::strtod(i, &value_last);
      i = value_last;
      tokens.push_back(value);
    } else {
      throw std::invalid_argument{std::string{"unexpected token: `"} + *i +
                                  "`"};
    }
  }
}
}  // namespace satyr::testing
