#include "../areo-include/matrix_exceptions.hpp"

InvalidDimsException::InvalidDimsException(std::size_t lhs_rows, std::size_t lhs_cols,
                                           std::size_t rhs_rows, std::size_t rhs_cols) {
    message_ = "Matrix dimension mismatch: left-hand matrix is " + std::to_string(lhs_rows) + "x" +
               std::to_string(lhs_cols) + ", right-hand matrix is " + std::to_string(rhs_rows) +
               "x" + std::to_string(rhs_cols) +
               ". Both matrices must have the same dimensions for this operation.";
}

const char* InvalidDimsException::what() const noexcept {
    return message_.c_str();
}

InvalidIndexException::InvalidIndexException(std::size_t row, std::size_t col, std::size_t rows,
                                             std::size_t cols) {
    message_ = "Matrix index out of bounds: requested (" + std::to_string(row) + ", " +
               std::to_string(col) + ") in a " + std::to_string(rows) + "x" + std::to_string(cols) +
               " matrix.";
}

const char* InvalidIndexException::what() const noexcept {
    return message_.c_str();
}
