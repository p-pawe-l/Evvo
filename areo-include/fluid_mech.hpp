#pragma once

#include <functional>
#include <initializer_list>
#include <cmath>

constexpr double g_acc = 9.81;
using DoubleFn = std::function<double(std::vector<double>)>;

/**
 * @brief Numerically approximates the partial derivative of a multivariable function
 *        using the forward-difference method.
 *
 * @param func      Function to differentiate; takes a vector of doubles, returns a double.
 * @param wrt_index Index into params identifying the variable to differentiate with respect to.
 * @param params    Coordinates of the point at which to evaluate the derivative.
 * @param dx        Step size used for the finite difference (default 0.0001).
 *
 * @return Approximate value of d(func)/d(params[wrt_index]) at the given point.
 */
template <typename... Params>
double calc_derivative(DoubleFn func, std::size_t wrt_index, Params... params, double dx = 0.0001) {
    std::vector<double> forward{static_cast<double>(params)...};
    std::vector<double> backward = forward;

    forward[wrt_index] += dx;
    return (func(forward) - func(backward)) / dx;
}
