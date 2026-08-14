#include "../areo-include/vector_3d.hpp"
#include <cmath>
#include <initializer_list>


Vector3D::Vector3D(double x, double y, double z) {
    vec_data_[0] = x;
    vec_data_[1] = y;
    vec_data_[2] = z;
}

Vector3D::Vector3D(std::initializer_list<double> init_list) {
    auto ptr = init_list.begin();
    for (std::size_t i = 0; i < 3; ++i) {
        vec_data_[i] = std::move(*(ptr++));
    }
}

Vector3D::Vector3D(std::vector<double> init_list) {
    for (std::size_t i = 0; i < 3; ++i) {
        vec_data_[i] = init_list[i];
    }
}

double Vector3D::dot_product(const Vector3D& other) const noexcept {
    return vec_data_[0] * other.vec_data_[0] +
           vec_data_[1] * other.vec_data_[1] +
           vec_data_[2] * other.vec_data_[2];
}

double Vector3D::dot_product(Vector3D&& other) const noexcept {
    return dot_product(other);
}

Vector3D Vector3D::cross_product(const Vector3D& other) const noexcept {
    return Vector3D(
        vec_data_[1] * other.vec_data_[2] - vec_data_[2] * other.vec_data_[1],
        vec_data_[2] * other.vec_data_[0] - vec_data_[0] * other.vec_data_[2],
        vec_data_[0] * other.vec_data_[1] - vec_data_[1] * other.vec_data_[0]
    );
}

Vector3D Vector3D::cross_product(Vector3D&& other) const noexcept {
    return cross_product(other);
}

double Vector3D::angle_beetwen(const Vector3D& other) const noexcept {
    return std::acos(dot_product(other) / (get_size() * other.get_size()));
}

double Vector3D::angle_beetwen(Vector3D&& other) const noexcept {
    return angle_beetwen(other);
}

void Vector3D::normalize() noexcept {
    double size = get_size();
    vec_data_[0] /= size;
    vec_data_[1] /= size;
    vec_data_[2] /= size;
}

Vector3D Vector3D::get_normalized() const noexcept {
    Vector3D result = *this;
    result.normalize();
    return result;
}

double Vector3D::get_size() const noexcept {
    return std::sqrt(vec_data_[0] * vec_data_[0] +
                      vec_data_[1] * vec_data_[1] +
                      vec_data_[2] * vec_data_[2]);
}

double Vector3D::get_x() const noexcept {
    return vec_data_[0];
}

double Vector3D::get_y() const noexcept {
    return vec_data_[1];
}

double Vector3D::get_z() const noexcept {
    return vec_data_[2];
}

