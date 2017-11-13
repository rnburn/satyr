#include <satyr/test/data_dictionary.h>

namespace satyr::testing {
//------------------------------------------------------------------------------
// lookup
//------------------------------------------------------------------------------
auto data_dictionary::lookup(std::string_view name) const
    -> std::optional<value_type> {
  auto iter = variables_.find(std::string{name});
  if (iter != std::end(variables_))
    return iter->second;
  else
    return {};
}

//------------------------------------------------------------------------------
// set
//------------------------------------------------------------------------------
void data_dictionary::set(std::string_view name, value_type value) {
  variables_.insert_or_assign(std::string{name}, std::move(value));
}
} // namespace satyr::testing
