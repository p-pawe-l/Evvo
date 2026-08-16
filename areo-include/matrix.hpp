/**
 * @file matrix.hpp
 * @brief Defines Matrix, a general-purpose 2D matrix.
 */

#pragma once
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <vector>


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
    template <class _InputIterator> 
    Matrix(_InputIterator first, _InputIterator last, std::size_t rows, std::size_t cols):
    matrix_(std::make_unique<double[]>(rows * cols)),
    rows_(rows),
    cols_(cols)
    {
        std::copy(first, last, matrix_.get());
    }
    explicit Matrix(const std::vector<std::vector<double>>& collection):
    Matrix(collection.begin(), collection.end(), collection.size(), collection.front().size())
    {}

    Matrix(std::initializer_list<std::initializer_list<double>> collection):
    Matrix(collection.begin(), collection.end(), collection.size(), collection.front().size())
    {}

    Matrix(Matrix<T>&&) noexcept = default;
    Matrix& operator=(Matrix<T>&&) noexcept = default;

    Matrix(const Matrix<T>& other): 
    Matrix(other.matrix_.get(), other.rows_, other.cols_)
    {}
    Matrix& operator=(const Matrix<T>& other)
    {
        return *this = Matrix(other);
    }

    void negate() noexcept 
    {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            matrix_[i] = -matrix_[i];
        }
    }
    Matrix get_negated() const noexcept 
    {
        Matrix temp(*this);
        temp.negate();
        return temp;
    }

    Matrix& operator+=(const Matrix<T>& other) 
    {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            matrix_[i] += other.matrix_;
        }
        return *this;
    }
    Matrix& operator-=(const Matrix<T>& other)
    {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            matrix_[i] -= other.matrix_;
        }
        return *this;
    }

};

