#pragma once

#include <vector>
#include <string>

namespace cnsl
{
    size_t selection(std::string_view message, const std::vector<std::string>& selections);
    bool boolean(std::string_view message);
    std::string read_line(std::string_view message, std::string_view default_line = "");

    void success_message(std::string_view message);
    void error_message(std::string_view message);
}
