/**
 * @file matrix.hpp
 * @brief Defines Matrix, a general-purpose 2D matrix.
 */

#pragma once

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <span>
#include <type_traits>
#include <vector>
#include <functional>

#include "matrix_exceptions.hpp"


template <typename T> requires std::is_arithmetic_v<T>
class Matrix {
private:
    std::unique_ptr<T[]> matrix_;
    std::size_t rows_, cols_;

public:
    /**
     * @brief Constructs a matrix by copying from a raw buffer.
     * @param data Pointer to at least rows*cols contiguous elements, stored row-major.
     * @param rows Number of rows.
     * @param cols Number of columns.
     */
    Matrix(const T* data, std::size_t rows, std::size_t cols):
    matrix_(std::make_unique<T[]>(rows * cols)),
    rows_(rows),
    cols_(cols)
    {
        std::copy(data, data + rows * cols, matrix_.get());
    }

    /**
     * @brief Constructs a matrix by copying from an iterator range.
     * @param first Iterator to the first element (row-major order).
     * @param last Iterator to one past the last element.
     * @param rows Number of rows.
     * @param cols Number of columns.
     */
    template <class _InputIterator>
    Matrix(_InputIterator first, _InputIterator last, std::size_t rows, std::size_t cols):
    matrix_(std::make_unique<T[]>(rows * cols)),
    rows_(rows),
    cols_(cols)
    {
        std::copy(first, last, matrix_.get());
    }

    /**
     * @brief Constructs a matrix from a vector of rows.
     * @param collection Row-major nested vector; all rows must be the same length.
     */
    explicit Matrix(const std::vector<std::vector<double>>& collection):
    Matrix(collection.begin(), collection.end(), collection.size(), collection.front().size())
    {}

    /**
     * @brief Constructs a matrix from a nested brace-init-list, e.g. {{1, 2}, {3, 4}}.
     * @param collection Row-major nested initializer list; all rows must be the same length.
     */
    Matrix(std::initializer_list<std::initializer_list<double>> collection):
    Matrix(collection.begin(), collection.end(), collection.size(), collection.begin()->size())
    {}

    /**
     * @brief Constructs a matrix with every element set to the same value.
     * @param value Fill value for all elements.
     * @param rows Number of rows.
     * @param cols Number of columns.
     */
    explicit Matrix(T value, std::size_t rows, std::size_t cols):
    matrix_(std::make_unique<T[]>(rows * cols)),
    rows_(rows),
    cols_(cols)
    {
        std::fill_n(matrix_, rows_ * cols_, value);
    }

    Matrix(Matrix<T>&&) noexcept = default;
    Matrix& operator=(Matrix<T>&&) noexcept = default;

    /** @brief Copy-constructs a matrix, deep-copying the underlying data. */
    Matrix(const Matrix<T>& other):
    Matrix(other.matrix_.get(), other.rows_, other.cols_)
    {}  

    /** @brief Copy-assigns a matrix, deep-copying the underlying data. */
    Matrix& operator=(const Matrix<T>& other)
    {
        return *this = Matrix(other);
    }

    /** @brief Returns a mutable view over the underlying row-major element buffer. */
    [[nodiscard]] std::span<T> data() noexcept
    {
        return {matrix_.get(), rows_ * cols_};
    }

    /** @brief Returns a read-only view over the underlying row-major element buffer. */
    [[nodiscard]] std::span<const T> data() const noexcept
    {
        return {matrix_.get(), rows_ * cols_};
    }

    /**
     * @brief Returns the element at the given row and column.
     * @param row Row index (0-based).
     * @param col Column index (0-based).
     * @throws InvalidIndexException If row or col is out of bounds for this matrix.
     */
    [[nodiscard]] T at(std::size_t row, std::size_t col) const
    {
        if (row > (rows_ - 1) || col > (cols_ - 1)) {
            throw InvalidIndexException(row, col, rows_, cols_);
        }
        return matrix_.get()[row * cols_ + col];
    }

    /** @brief Negates every element of this matrix in place. */
    void negate() noexcept
    {
        std::ranges::transform(
            data(),
            matrix_.get(),
            std::negate<>{}
        );
    }

    /** @brief Returns a copy of this matrix with every element negated. */
    [[nodiscard]] Matrix get_negated() const noexcept
    {
        Matrix temp(*this); // maybe can be optimized in order not to use copy constructor ?
        temp.negate();
        return temp;
    }

    /**
     * @brief Adds another matrix to this one element-wise, in place.
     * @param other Matrix of the same dimensions.
     * @return Reference to this matrix.
     */
    Matrix& operator+=(const Matrix<T>& other)
    {   
        std::ranges::transform(
            data(),        
            other.data(),
            matrix_.get(),                                     
            std::plus<>{}                                      
        );
        return *this;
    }

    /**
     * @brief Adds a scalar to every element of this matrix, in place.
     * @param x Scalar to add.
     * @return Reference to this matrix.
     */
    Matrix& operator+=(T x)
    {
        std::ranges::transform(
            data(),
            matrix_.get(),
            [x](T element) { return element + x; }
        );
        return *this;
    }

    /**
     * @brief Subtracts another matrix from this one element-wise, in place.
     * @param other Matrix of the same dimensions.
     * @return Reference to this matrix.
     */
    Matrix& operator-=(const Matrix<T>& other)
    {
        std::ranges::transform(
            data(),
            other.data(),
            matrix_.get(),
            std::minus<>{}
        );
        return *this;
    }

    /**
     * @brief Subtracts a scalar from every element of this matrix, in place.
     * @param x Scalar to subtract.
     * @return Reference to this matrix.
     */
    Matrix& operator-=(T x)
    {
        std::ranges::transform(
            data(),
            matrix_.get(),
            [x](T element){ return element - x; }
        );
        return *this;
    }

    /**
     * @brief Multiplies this matrix by another, in place.
     * @param other Matrix to multiply by.
     * @return Reference to this matrix.
     */
    Matrix& operator*=(const Matrix<T>& other)
    {
        // matrix multiplication operation
    }

    /**
     * @brief Multiplies every element of this matrix by a scalar, in place.
     * @param x Scalar to multiply by.
     * @return Reference to this matrix.
     */
    Matrix& operator*=(T x)
    {
        std::ranges::transform(
            data(),
            matrix_.get(),
            [x](T element) { return element * x; }
        );
        return *this;
    }

    /** @brief Transposes this matrix in place, swapping rows and columns. */
    void transpose() noexcept
    {
        // swapping number of rows and columns
        rows_ ^= cols_;
        cols_ ^= rows_; 
        rows_ ^= cols_;

        // Transposing matrix logic 
               
    }

    /** @brief Returns a transposed copy of this matrix, leaving this matrix unchanged. */
    [[nodiscard]] Matrix get_transposed() const noexcept
    {
        Matrix temp(*this);
        temp.transpose();
        return temp;
    }
};

