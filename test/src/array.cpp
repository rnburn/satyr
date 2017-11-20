#include "array.h"
#include <algorithm>

namespace satyr::testing {
//------------------------------------------------------------------------------
// parse_vector
//------------------------------------------------------------------------------
std::tuple<std::vector<double>, std::vector<size_t>> parse_vector(
    token_stream& stream) {
  std::vector<double> result;
  std::vector<size_t> dimensions(1);
  stream.consume<l_bracket_token>();
  while (1) {
    if (!stream.peek<double>())
      break;
    result.push_back(stream.consume<double>());
    if (stream.peek<comma_token>()) stream.consume<comma_token>();
  }
  stream.consume<r_bracket_token>();
  dimensions[0] = result.size();
  return {std::move(result), std::move(dimensions)};
}

//------------------------------------------------------------------------------
// parse_array
//------------------------------------------------------------------------------
std::tuple<std::vector<double>, std::vector<size_t>> parse_array(
    size_t num_dimensions, token_stream& stream) {
  if (num_dimensions == 1)
    return parse_vector(stream);
  std::vector<double> result;
  std::vector<size_t> dimensions(num_dimensions);

  stream.consume<l_bracket_token>();
  if (stream.peek<r_bracket_token>()) {
    stream.consume<r_bracket_token>();
    return {std::move(result), std::move(dimensions)};
  }

  auto& n = dimensions[0];
  ++n;
  auto [inner_values1, inner_dimensions1] = parse_array(num_dimensions-1, stream);
  if (stream.peek<comma_token>()) stream.consume<comma_token>();

  std::copy(std::begin(inner_dimensions1), std::end(inner_dimensions1),
            std::next(std::begin(dimensions)));
  std::copy(std::begin(inner_values1), std::end(inner_values1),
            std::back_inserter(result));

  while (1) {
    if (stream.peek<r_bracket_token>()) break;
    ++n;
    auto [inner_values2, inner_dimensions2] =
        parse_array(num_dimensions - 1, stream);
    if (inner_dimensions1 != inner_dimensions2)
      throw std::invalid_argument{"array dimensions are not consistent"};
    std::copy(std::begin(inner_values2), std::end(inner_values2),
              std::back_inserter(result));
    if (stream.peek<comma_token>()) stream.consume<comma_token>();
  }

  stream.consume<r_bracket_token>();
  return {std::move(result), std::move(dimensions)};
}
} // namespace satyr::testing
