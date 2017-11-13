#include <satyr/test/data_dictionary.h>
#include <tuple>
#include <stdexcept>
#include <cctype>
#include <algorithm>

namespace satyr::testing {
//------------------------------------------------------------------------------
// starts_with
//------------------------------------------------------------------------------
static bool starts_with(std::string_view s1, std::string_view s2) {
  return s1.compare(0, s2.size(), s2) == 0;
}

//------------------------------------------------------------------------------
// get_next_statement
//------------------------------------------------------------------------------
std::tuple<std::string_view, std::string_view> get_next_statement(
    std::string_view source) {
  auto i = std::begin(source);
  auto last = std::end(source);

  // Skip over comments
  while (1) {
    // Skip over whitespace
    i = std::find_if_not(i, last, [](char c) { return std::isspace(c); });
    if (i == last) return {};

    if (!starts_with(std::string_view(&*i, std::distance(i, last)), "//")) {
      break;
    }

    i = std::find(i, last, '\n');
  }

  // Get the end of the statement
  auto statement_first = i;
  auto statement_last = std::find(i, last, ';');
  if (statement_last == last) throw std::invalid_argument{"invalid statement"};
  i = std::next(statement_last);
  auto statement = std::string_view(
      &*statement_first, std::distance(statement_first, statement_last));
  auto rest = std::string_view(&*i, std::distance(i, last));
  return std::make_tuple(statement, rest);
}

//------------------------------------------------------------------------------
// parse_identifier
//------------------------------------------------------------------------------
std::tuple<std::string_view, std::string_view> parse_identifier(
    std::string_view statement) {
  auto i = std::begin(statement);
  auto last = std::end(statement);
  // Skip over whitespace
  i = std::find_if_not(i, last, [](char c) { return std::isspace(c); });
  if (i == last) throw std::invalid_argument{"invalid identifier"};

  auto identifier_first = i;
  auto identifier_last =
      std::find_if(i, last, [](char c) { return std::isspace(c); });
  i = std::next(identifier_last);
  auto identifier = std::string_view(
      &*identifier_first, std::distance(identifier_first, identifier_last));
  auto rest =
      std::string_view(&*i, std::distance(i, last));
  return std::make_tuple(identifier, rest);
}

//------------------------------------------------------------------------------
// consume
//------------------------------------------------------------------------------
std::string_view consume(std::string_view source, std::string_view s) {
  auto i = std::begin(source);
  auto last = std::end(source);
  i = std::find_if_not(i, last, [](char c) { return std::isspace(c); });

  auto rest = std::string_view(&*i, std::distance(i, last));
  if (!starts_with(rest, s))
    throw std::invalid_argument{"unexpected token"};
  return rest.substr(s.size());
}

//------------------------------------------------------------------------------
// parse_data_dictionary
//------------------------------------------------------------------------------
static data_dictionary parse_data_dictionary(std::string_view s) {
  data_dictionary result;
  while (1) {
    auto [statement, rest] = get_next_statement(s);
    if (statement.empty()) return result;
    std::string_view type, name, value;
    std::tie(type, statement) = parse_identifier(statement);
    std::tie(name, statement) = parse_identifier(statement);
    value = consume(statement, "=");

    // TODO(rnburn): add entry
  }
}

//------------------------------------------------------------------------------
// read_data_dictionaries
//------------------------------------------------------------------------------
std::vector<data_dictionary> read_data_dictionaries(std::istream istream) {
  std::vector<data_dictionary> result;
  return result;
}
} // namespace satyr::testing
