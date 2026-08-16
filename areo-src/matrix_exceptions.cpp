#include "../areo-include/matrix_exceptions.hpp"

InvalidDimsException::InvalidDimsException(
    std::size_t lhs_rows, std::size_t lhs_cols,
    std::size_t rhs_rows, std::size_t rhs_cols
) {
    message_ = "Matrix dimension mismatch: left-hand matrix is "
        + std::to_string(lhs_rows) + "x" + std::to_string(lhs_cols)
        + ", right-hand matrix is "
        + std::to_string(rhs_rows) + "x" + std::to_string(rhs_cols)
        + ". Both matrices must have the same dimensions for this operation.";
}

const char* InvalidDimsException::what() const noexcept {
    return message_.c_str();
}
