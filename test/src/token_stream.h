#pragma once

#include <variant>
#include <vector>
#include <string_view>
#include <stdexcept>

namespace satyr::testing {
struct l_bracket_token {};
struct r_bracket_token {};
struct comma_token {};
struct semicolon_token {};
struct equals_token {};
struct divider_token {};

using token =
    std::variant<std::string_view, double, l_bracket_token, r_bracket_token,
                 semicolon_token, comma_token, equals_token, divider_token>;

//------------------------------------------------------------------------------
// token_stream
//------------------------------------------------------------------------------
class token_stream {
 public:
  explicit token_stream(std::vector<token>&& tokens)
      : tokens_{std::move(tokens)} {}

  template <class T>
  T consume() {
    if (empty())
      throw std::invalid_argument{"unexpected eof"};
    auto value_ptr = std::get_if<T>(&tokens_[position_]);
    if (!value_ptr)
      throw std::invalid_argument{"unexpected token"};
    ++position_;
    return *value_ptr;
  }

  template <class T>
  void expect() {
    if (empty())
      throw std::invalid_argument{"unexpected eof"};
    if (!std::get_if<T>(&tokens_[position_])) 
      throw std::invalid_argument{"unexpected token"};
  }

  token peek() const {
    if (empty())
      throw std::invalid_argument{"unexpected eof"};
    return tokens_[position_];
  }

  template <class T>
  bool peek() const {
    if (empty()) return false;
    return std::get_if<T>(&tokens_[position_]);
  }

  bool empty() const {
    return position_ == tokens_.size();
  }
 private:
  int position_ = 0;
  std::vector<token> tokens_;
};
} // namespace satyr::testing
