#pragma once

namespace satyr::access_mode {
const struct readonly {} readonly_v{};
const struct readwrite {} readwrite_v{};
const struct raw {} raw_v{};
} // namespace satyr::access_mode
