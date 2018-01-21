#include <satyr/log_context.h>
#include <cassert>
#include <iostream>

int main() {
  auto& context_manager = satyr::log_context_manager::instance();
  assert(context_manager.active_contexts().empty());
  {
    [[maybe_unused]] auto log_context1 = satyr::log_context{"abc", 123};
    assert(context_manager.active_contexts() == "abc=123;");
    {
      [[maybe_unused]] auto log_context2 = satyr::log_context{"xy", 42};
      assert(context_manager.active_contexts() == "abc=123;xy=42;");
    }
    assert(context_manager.active_contexts() == "abc=123;");
  }
  assert(context_manager.active_contexts().empty());
  return 0;
}
