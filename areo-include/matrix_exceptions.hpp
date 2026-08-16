/**
 * @file matrix_exceptions.hpp
 * @brief Defines exception types thrown by Matrix operations.
 */

#pragma once

#include <cstddef>
#include <exception>
#include <string>

/**
 * @brief Thrown when a matrix operation receives operands whose dimensions
 *        are incompatible with the operation being performed.
 */
class InvalidDimsException : public std::exception {
public:
    /**
     * @brief Constructs the exception with the mismatched dimensions.
     * @param lhs_rows Row count of the left-hand matrix.
     * @param lhs_cols Column count of the left-hand matrix.
     * @param rhs_rows Row count of the right-hand matrix.
     * @param rhs_cols Column count of the right-hand matrix.
     */
    InvalidDimsException(
        std::size_t lhs_rows, std::size_t lhs_cols,
        std::size_t rhs_rows, std::size_t rhs_cols
    );

    /**
     * @brief Returns a message describing the dimension mismatch.
     * @return Null-terminated explanatory string.
     */
    [[nodiscard]] const char* what() const noexcept override;

private:
    std::string message_;
};
