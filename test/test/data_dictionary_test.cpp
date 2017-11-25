#include <satyr/test/data_dictionary.h>
#include <sstream>
#include <cassert>
using namespace satyr::testing;

int main() {
  {
    std::istringstream iss;
    auto data_dictionaries = read_data_dictionaries(iss);
    assert(data_dictionaries.empty());
  }

  {
    std::istringstream iss{R"(
  double x = 1.0;
  double y = 2.0;
  )"};
    auto data_dictionaries = read_data_dictionaries(iss);
    assert(data_dictionaries.size() == 1);
    assert(get<double>(data_dictionaries[0], "x") == 1.0);
    assert(get<double>(data_dictionaries[0], "y") == 2.0);
  }

  {
    std::istringstream iss{R"(
  double x = 1.0;
//---
  double x = 2.0;
  )"};
    auto data_dictionaries = read_data_dictionaries(iss);
    assert(data_dictionaries.size() == 2);
    assert(get<double>(data_dictionaries[0], "x") == 1.0);
    assert(get<double>(data_dictionaries[1], "x") == 2.0);
  }

  {
    std::istringstream iss{R"(
  vector v = {1, 2, 3};
  matrix m = {{1, 2, 3}, {4, 5, 6}};
  symmetric_matrix s = {{1, 5}, {5, 1}};
  )"};
    auto data_dictionaries = read_data_dictionaries(iss);
    assert(data_dictionaries.size() == 1);

    auto v = get<satyr::vector<double>>(data_dictionaries[0], "v");
    assert((v == satyr::vector<double>{1, 2, 3}));

    auto m = get<satyr::matrix<double>>(data_dictionaries[0], "m");
    assert((m == satyr::matrix<double>{{1, 2, 3}, {4, 5, 6}}));

    auto s = get<satyr::symmetric_matrix<double>>(data_dictionaries[0], "s");
    assert((s == satyr::symmetric_matrix<double>{{1, 5}, {5, 1}}));
  }
  return 0;
}
