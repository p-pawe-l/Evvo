#include "../areo-include/matrix.hpp"
#include <cstring>
#include <utility>


Matrix::Matrix(init_matrix_t init_list) {
    rows_ = init_list.size();
    if (rows_ == 0) {
        // throw error here 
    }
    cols_ = (*init_list.begin()).size();
    if (cols_ == 0) {
        // throw error here 
    }

    matrix_ = new double[rows_ * cols_];
    auto row_ptr = init_list.begin();
    for (std::size_t i = 0; i < rows_; ++i, ++row_ptr) {
        auto col_ptr = row_ptr->begin();
        for (std::size_t j = 0; j < cols_; ++j, ++col_ptr) {
            matrix_[i * cols_ + j] = std::move(*col_ptr);
        }
    }
}

Matrix::Matrix(const Matrix& other) {
    if (other.matrix_ != nullptr) {
        if (this->matrix_ == nullptr) {
            this->matrix_ = new double[other.rows_ * other.cols_];
        }

        std::memcpy(
            this->matrix_, other.matrix_, other.cols_ * other.rows_ * sizeof(double)
        );
        this->rows_ = other.rows_;
        this->cols_ = other.cols_;
    }
    
    // throw error here
}

Matrix::Matrix(Matrix&& other) {
    if (&other != this) {
        this->matrix_ = other.matrix_;
        this->rows_ = other.rows_;
        this->cols_ = other.cols_;

        other.matrix_ = nullptr;
        other.rows_ = 0;
        other.cols_ = 0;
    }
}