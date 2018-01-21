#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <cstdint>

namespace satyr {
class log_context_manager {
 public:
   static log_context_manager& instance();

   void push(std::string_view key, std::string_view value);

   void pop();

   std::string_view active_contexts() const { return active_contexts_; }
 private:
  std::string active_contexts_;
  std::vector<size_t> sizes_;
};

class log_context {
 public:
   log_context(std::string_view key, std::string_view value);

   log_context(std::string_view key, int64_t value);

   log_context(const log_context&) = delete;
   log_context(log_context&&);

   log_context& operator=(const log_context&) = delete;
   log_context& operator=(log_context&&);

   ~log_context();
 private:
   bool should_pop_ = true;
};
} // namespace satyr
