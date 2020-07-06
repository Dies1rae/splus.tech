#pragma once

#include <string>
#include <string_view>

namespace splus {
    bool replace(std::string& text, std::string_view what, std::string_view by);
    size_t replaceAll(std::string& text, std::string_view what, std::string_view by);
}
