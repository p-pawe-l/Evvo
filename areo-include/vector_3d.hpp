/**
 * @file vector_3d.hpp
 * @brief Defines Vector3D, a 3-component mathematical vector with dot
 *        product, size, and normalization operations.
 */

#pragma once

#include <initializer_list>
#include <vector>

/**
 * @brief 3-component mathematical vector.
 */
class Vector3D {
private:
    double vec_data_[3];

public:
    Vector3D() = default;

    /**
     * @brief Constructs a vector from its x, y, z components.
     * @param x X component.
     * @param y Y component.
     * @param z Z component.
     */
    Vector3D(double x, double y, double z);

    /**
     * @brief Constructs a vector from an initializer list of components.
     * @param init_list Components in {x, y, z} order.
     */
    Vector3D(std::initializer_list<double> init_list);

    /**
     * @brief Constructs a vector from a vector of components.
     * @param init_list Components in {x, y, z} order.
     */
    Vector3D(std::vector<double> init_list);

    ~Vector3D() = default;

    /**
     * @brief Computes the dot product with another vector.
     * @param other Vector to dot with.
     * @return The dot product of this vector and other.
     */
    [[nodiscard]] double dot_product(const Vector3D& other) const noexcept;

    /**
     * @brief Computes the dot product with another vector.
     * @param other Vector to dot with (consumed by value).
     * @return The dot product of this vector and other.
     */
    [[nodiscard]] double dot_product(Vector3D&& other) const noexcept;

    /**
     * @brief Computes the cross product with another vector.
     * @param other Vector to cross with.
     * @return The cross product of this vector and other.
     */
    [[nodiscard]] Vector3D cross_product(const Vector3D& other) const noexcept;

    /**
     * @brief Computes the cross product with another vector.
     * @param other Vector to cross with (consumed by value).
     * @return The cross product of this vector and other.
     */
    [[nodiscard]] Vector3D cross_product(Vector3D&& other) const noexcept;

    /**
     * @brief Computes the angle between this vector and another.
     * @param other Vector to measure the angle against.
     * @return The angle between the two vectors, in radians.
     */
    [[nodiscard]] double angle_beetwen(const Vector3D& other) const noexcept;

    /**
     * @brief Computes the angle between this vector and another.
     * @param other Vector to measure the angle against (consumed by value).
     * @return The angle between the two vectors, in radians.
     */
    [[nodiscard]] double angle_beetwen(Vector3D&& other) const noexcept;

    /**
     * @brief Normalizes this vector in place to unit length.
     */
    void normalize() noexcept;

    /**
     * @brief Returns a unit-length copy of this vector.
     * @return A normalized copy of this vector.
     */
    [[nodiscard]] Vector3D get_normalized() const noexcept;

    /**
     * @brief Computes the magnitude (length) of this vector.
     * @return The vector's magnitude.
     */
    [[nodiscard]] double get_size() const noexcept;

    /**
     * @brief Returns the x component of this vector.
     * @return The x component.
     */
    [[nodiscard]] double get_x() const noexcept;

    /**
     * @brief Returns the y component of this vector.
     * @return The y component.
     */
    [[nodiscard]] double get_y() const noexcept;

    /**
     * @brief Returns the z component of this vector.
     * @return The z component.
     */
    [[nodiscard]] double get_z() const noexcept;
};