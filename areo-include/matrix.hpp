/**
 * @file matrix.hpp
 * @brief Defines Matrix, a general-purpose 2D matrix.
 */

#pragma once
#include <algorithm>
#include <memory>
#include <type_traits>


template <typename T> requires std::is_arithmetic_v<T>
class Matrix {
private:
    std::unique_ptr<T[]> matrix_;
    std::size_t rows_, cols_;

public:
    Matrix(const T* data, std::size_t rows, std::size_t cols):
    matrix_(std::make_unique<double[]>(rows * cols)),
    rows_(rows),
    cols_(cols)
    {
        std::copy(data, data + rows * cols, matrix_.get());
    }

    Matrix(Matrix<T>&&) noexcept = default;
    Matrix& operator=(Matrix<T>&&) noexcept = default;

    Matrix(const Matrix<T>& other): 
    Matrix(other.matrix_.get(), other.rows_, other.cols_)
    {}

    Matrix& operator=(const Matrix<T>& other)
    {
        return *this = Matrix(other);
    }
};

