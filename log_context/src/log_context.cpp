#include <satyr/log_context.h>
#include <exception>
#include <iostream>

namespace satyr {
log_context_manager& log_context_manager::instance() {
  static thread_local log_context_manager manager;
  return manager;
}

void log_context_manager::push(std::string_view key, std::string_view value) {
     auto size = key.size() + 1/*=*/ + value.size() + 1/*;*/;
     active_contexts_.reserve(active_contexts_.size() + size);
     sizes_.push_back(size);
     active_contexts_.append(key);
     active_contexts_.push_back('=');
     active_contexts_.append(value);
     active_contexts_.push_back(';');
}

void log_context_manager::pop() {
     if (sizes_.empty()) {
       std::cerr << "corrupt log_context!\n";
       std::terminate();
     }
     active_contexts_.erase(active_contexts_.size() - sizes_.back());
     sizes_.pop_back();
}

log_context::log_context(std::string_view key, std::string_view value) {
  log_context_manager::instance().push(key, value);
}

log_context::log_context(std::string_view key, int64_t value) {
  log_context_manager::instance().push(key, std::to_string(value));
}

log_context::log_context(log_context&& other) 
  : should_pop_{other.should_pop_}
{
  other.should_pop_ = false;
}

log_context& log_context::operator=(log_context&& other) {
  should_pop_ = other.should_pop_;
  other.should_pop_ = false;
  return *this;
}

log_context::~log_context() {
  if (should_pop_) log_context_manager::instance().pop();
}
} // namespace satyr
