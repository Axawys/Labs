#pragma once

#include <stdexcept>
#include <string>

class StackEmptyException : public std::runtime_error {
public:
    explicit StackEmptyException(const std::string& msg)
        : std::runtime_error(msg) {}
};
