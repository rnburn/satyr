#pragma once

#include <variant>
#include <string_view>
#include <satyr/linear_algebra.h>
#include <map>
#include <optional>
#include <iostream>
#include <vector>

namespace satyr::testing {
//------------------------------------------------------------------------------
// data_dictionary
//------------------------------------------------------------------------------
class data_dictionary {
 public:
  using value_type =
      std::variant<std::string, double, satyr::vector<double>,
                   satyr::matrix<double>, satyr::symmetric_matrix<double>,
                   satyr::lower_triangular_matrix<double>,
                   satyr::upper_triangular_matrix<double>>;

  std::optional<value_type> lookup(std::string_view name) const;

  void set(std::string_view name, value_type value);
 private:
  std::map<std::string, value_type> variables_;
};

//------------------------------------------------------------------------------
// read_data_dictionaries
//------------------------------------------------------------------------------
std::vector<data_dictionary> read_data_dictionaries(std::istream& istream);
} // namespace satyr::testing
