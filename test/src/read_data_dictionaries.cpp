#include <satyr/linear_algebra.h>
#include <satyr/test/data_dictionary.h>
#include "lex.h"
#include "array.h"
#include <tuple>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <iterator>

namespace satyr::testing {
//------------------------------------------------------------------------------
// compute_matrix_subshape
//------------------------------------------------------------------------------
static satyr::subshape<2> compute_matrix_subshape(
    const std::vector<size_t>& dimensions) {
  index_t m = static_cast<index_t>(dimensions[0]);
  index_t n = static_cast<index_t>(dimensions[1]);
  return {satyr::shape(m, n), {n, 1}};
}

//------------------------------------------------------------------------------
// parse_value
//------------------------------------------------------------------------------
static data_dictionary::value_type parse_value(std::string_view type,
                                        testing::token_stream& token_stream) {
  if (type == "double") {
    return token_stream.consume<double>();
  } else if (type == "string") {
    return std::string{token_stream.consume<std::string_view>()};
  } else if (type == "vector") {
    auto [values, dimensions] = parse_array(1, token_stream);
    satyr::vector<double> vector = satyr::vector_view<double>{
        values.data(), satyr::shape(static_cast<index_t>(values.size()))};
    return vector;
  } else if (type == "matrix") {
    auto [values, dimensions] = parse_array(2, token_stream);
    satyr::matrix<double> matrix = satyr::matrix_subview<double>{
        values.data(), compute_matrix_subshape(dimensions)};
    return matrix;
  } else if (type == "lower_triangular_matrix") {
    auto [values, dimensions] = parse_array(2, token_stream);
    satyr::lower_triangular_matrix<double> matrix =
        satyr::lower_triangular_matrix_subview<double>{
            values.data(), compute_matrix_subshape(dimensions)};
    /* return matrix; */
  } else if (type == "upper_triangular_matrix") {
  } else if (type == "symmetric_matrix") {
  } else {
    throw std::invalid_argument{std::string{"unknown type `"} +
                                std::string{type} + "`"};
  }
}


//------------------------------------------------------------------------------
// read_data_dictionaries
//------------------------------------------------------------------------------
std::vector<data_dictionary> read_data_dictionaries(std::istream& istream) {
  std::vector<data_dictionary> result;
  auto source = std::string{std::istreambuf_iterator<char>(istream), {}};
  auto token_stream = lex(source);

  // Remove any initial divider.
  if (token_stream.peek<divider_token>()) token_stream.consume<divider_token>();

  while (1) {
    data_dictionary dictionary;
    while (1) {
      if (!token_stream.peek<std::string_view>())
        break;
      auto type = token_stream.consume<std::string_view>();
      auto key = token_stream.consume<std::string_view>();
      token_stream.consume<equals_token>();
    }
    if (token_stream.empty())
      break;
    token_stream.consume<divider_token>();
    result.emplace_back(std::move(dictionary));
  }

  return result;
}
} // namespace satyr::testing
