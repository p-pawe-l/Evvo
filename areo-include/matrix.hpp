/**
 * @file matrix.hpp
 * @brief Defines Matrix, a general-purpose 2D matrix with arithmetic,
 *        inversion, determinant, and transpose operations.
 */

#pragma once

#include <initializer_list>

/**
 * @brief General-purpose mathematical matrix.
 */
class Matrix {
    typedef std::initializer_list<std::initializer_list<double>> init_matrix_t;

private:
    double* matrix_;
    std::size_t rows_;
    std::size_t cols_;

public:
    /**
     * @brief Constructs a matrix from nested initializer lists of rows.
     * @param init Row-major nested initializer list of matrix entries.
     */
    explicit Matrix(init_matrix_t init);

    /**
     * @brief Constructs a matrix as a copy of another matrix.
     * @param other Matrix to copy.
     */
    explicit Matrix(const Matrix& other);

    /**
     * @brief Constructs a matrix by moving from another matrix.
     * @param other Matrix to move from.
     */
    explicit Matrix(Matrix&& other);

    /**
     * @brief Copy-assigns this matrix, deep-copying other's data.
     * @param other Matrix to copy from.
     * @return The resulting matrix.
     */
    Matrix operator=(const Matrix& other);

    /**
     * @brief Move-assigns this matrix, stealing other's data.
     * @param other Matrix to move from.
     * @return The resulting matrix.
     */
    Matrix operator=(Matrix&& other);

    ~Matrix();

    /**
     * @brief Computes the element-wise sum with another matrix.
     * @param other Matrix to add.
     * @return The sum of this matrix and other.
     */
    Matrix operator+(const Matrix& other);

    /**
     * @brief Computes the element-wise sum with another matrix.
     * @param other Matrix to add (consumed by value).
     * @return The sum of this matrix and other.
     */
    Matrix operator+(Matrix&& other);

    /**
     * @brief Adds a scalar to every element of this matrix.
     * @param diff Scalar value to add.
     * @return The resulting matrix.
     */
    Matrix operator+(double diff);

    /**
     * @brief Computes the element-wise difference with another matrix.
     * @param other Matrix to subtract.
     * @return The difference of this matrix and other.
     */
    Matrix operator-(const Matrix& other);

    /**
     * @brief Computes the element-wise difference with another matrix.
     * @param other Matrix to subtract (consumed by value).
     * @return The difference of this matrix and other.
     */
    Matrix operator-(Matrix&& other);

    /**
     * @brief Subtracts a scalar from every element of this matrix.
     * @param diff Scalar value to subtract.
     * @return The resulting matrix.
     */
    Matrix operator-(double diff);

    /**
     * @brief Computes the matrix product with another matrix.
     * @param other Matrix to multiply with.
     * @return The product of this matrix and other.
     */
    Matrix operator*(const Matrix& other);

    /**
     * @brief Computes the matrix product with another matrix.
     * @param other Matrix to multiply with (consumed by value).
     * @return The product of this matrix and other.
     */
    Matrix operator*(Matrix&& other);

    /**
     * @brief Multiplies every element of this matrix by a scalar.
     * @param scalar Scalar value to multiply by.
     * @return The resulting matrix.
     */
    Matrix operator*(double scalar);

    /**
     * @brief Inverts this matrix in place.
     */
    void inverse();

    /**
     * @brief Returns an inverted copy of this matrix.
     * @return The inverse of this matrix.
     */
    Matrix get_inversed();

    /**
     * @brief Computes the determinant of this matrix.
     * @return The determinant.
     */
    [[nodiscard]] double det();

    /**
     * @brief Transposes this matrix in place.
     */
    void transpose();

    /**
     * @brief Returns a transposed copy of this matrix.
     * @return The transpose of this matrix.
     */
    Matrix get_transposed();
};
