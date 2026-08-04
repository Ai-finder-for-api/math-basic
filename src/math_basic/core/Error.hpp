/**
 * @file Error.hpp
 * @brief Custom exception types for the math library.
 */
#pragma once
#include <stdexcept>
#include <string>

namespace qmath {

class MathError : public std::runtime_error {
public:
    explicit MathError(const std::string& msg) : std::runtime_error("MathError: " + msg) {}
};

class DimensionError : public MathError {
public:
    explicit DimensionError(const std::string& msg) : MathError("DimensionError: " + msg) {}
};

class ConvergenceError : public MathError {
public:
    explicit ConvergenceError(const std::string& msg) : MathError("ConvergenceError: " + msg) {}
};

} // namespace qmath
