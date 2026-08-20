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
    InvalidDimsException(std::size_t lhs_rows, std::size_t lhs_cols, std::size_t rhs_rows,
                         std::size_t rhs_cols);

    /**
     * @brief Returns a message describing the dimension mismatch.
     * @return Null-terminated explanatory string.
     */
    [[nodiscard]] const char* what() const noexcept override;

private:
    std::string message_;
};

/**
 * @brief Thrown when a row or column index passed to a Matrix accessor
 *        is out of bounds for that matrix's dimensions.
 */
class InvalidIndexException : public std::exception {
public:
    /**
     * @brief Constructs the exception with the offending index and matrix dimensions.
     * @param row Row index that was requested.
     * @param col Column index that was requested.
     * @param rows Row count of the matrix.
     * @param cols Column count of the matrix.
     */
    InvalidIndexException(std::size_t row, std::size_t col, std::size_t rows, std::size_t cols);

    /**
     * @brief Returns a message describing the out-of-bounds index.
     * @return Null-terminated explanatory string.
     */
    [[nodiscard]] const char* what() const noexcept override;

private:
    std::string message_;
};
