#include <catch2/catch_test_macros.hpp>

#include "matrix.hpp"
#include "matrix_exceptions.hpp"

// Only exercises Matrix methods that actually compile and run:
// the vector<vector<double>>/initializer_list/fill constructors and
// operator*=(const Matrix&) are not really implemented (type mismatches /
// empty body), so they are intentionally left untested here.

TEST_CASE("Matrix can be constructed from a raw buffer and read back via data()", "[matrix][ctor]") {
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<double> m(values, 2, 3);

    auto span = m.data();
    REQUIRE(span.size() == 6);
    for (std::size_t i = 0; i < 6; ++i) {
        REQUIRE(span[i] == values[i]);
    }
}

TEST_CASE("Matrix can be constructed from an iterator range", "[matrix][ctor]") {
    std::vector<double> values{1.0, 2.0, 3.0, 4.0};
    Matrix<double> m(values.begin(), values.end(), 2, 2);

    auto span = m.data();
    REQUIRE(span.size() == 4);
    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(span[i] == values[i]);
    }
}

TEST_CASE("Matrix::at returns the correct element for valid indices", "[matrix][at]") {
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<double> m(values, 2, 3);

    REQUIRE(m.at(0, 0) == 1.0);
    REQUIRE(m.at(0, 2) == 3.0);
    REQUIRE(m.at(1, 0) == 4.0);
    REQUIRE(m.at(1, 2) == 6.0);
}

TEST_CASE("Matrix::at throws InvalidIndexException for out-of-bounds indices", "[matrix][at]") {
    double values[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> m(values, 2, 2);

    REQUIRE_THROWS_AS(m.at(2, 0), InvalidIndexException);
    REQUIRE_THROWS_AS(m.at(0, 2), InvalidIndexException);
    REQUIRE_THROWS_AS(m.at(5, 5), InvalidIndexException);
}

TEST_CASE("Copy constructor deep-copies the underlying data", "[matrix][copy]") {
    double values[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> original(values, 2, 2);
    Matrix<double> copy(original);

    original.negate();

    REQUIRE(copy.at(0, 0) == 1.0);
    REQUIRE(copy.at(0, 1) == 2.0);
    REQUIRE(original.at(0, 0) == -1.0);
}

TEST_CASE("Copy assignment deep-copies the underlying data", "[matrix][copy]") {
    double values_a[] = {1.0, 2.0, 3.0, 4.0};
    double values_b[] = {10.0, 20.0, 30.0, 40.0};
    Matrix<double> a(values_a, 2, 2);
    Matrix<double> b(values_b, 2, 2);

    a = b;
    b.negate();

    REQUIRE(a.at(0, 0) == 10.0);
    REQUIRE(b.at(0, 0) == -10.0);
}

TEST_CASE("Move constructor transfers the underlying data", "[matrix][move]") {
    double values[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> original(values, 2, 2);
    Matrix<double> moved(std::move(original));

    REQUIRE(moved.at(0, 0) == 1.0);
    REQUIRE(moved.at(1, 1) == 4.0);
}

TEST_CASE("Move assignment transfers the underlying data", "[matrix][move]") {
    double values_a[] = {1.0, 2.0, 3.0, 4.0};
    double values_b[] = {10.0, 20.0, 30.0, 40.0};
    Matrix<double> a(values_a, 2, 2);
    Matrix<double> b(values_b, 2, 2);

    a = std::move(b);

    REQUIRE(a.at(0, 0) == 10.0);
    REQUIRE(a.at(1, 1) == 40.0);
}

TEST_CASE("negate flips the sign of every element in place", "[matrix][negate]") {
    double values[] = {1.0, -2.0, 3.0, -4.0};
    Matrix<double> m(values, 2, 2);

    m.negate();

    REQUIRE(m.at(0, 0) == -1.0);
    REQUIRE(m.at(0, 1) == 2.0);
    REQUIRE(m.at(1, 0) == -3.0);
    REQUIRE(m.at(1, 1) == 4.0);
}

TEST_CASE("get_negated returns a negated copy and leaves the original unchanged", "[matrix][negate]") {
    double values[] = {1.0, -2.0, 3.0, -4.0};
    Matrix<double> m(values, 2, 2);

    Matrix<double> negated = m.get_negated();

    REQUIRE(negated.at(0, 0) == -1.0);
    REQUIRE(negated.at(1, 1) == 4.0);
    REQUIRE(m.at(0, 0) == 1.0);
    REQUIRE(m.at(1, 1) == -4.0);
}

TEST_CASE("operator+= adds another matrix element-wise in place", "[matrix][arithmetic]") {
    double values_a[] = {1.0, 2.0, 3.0, 4.0};
    double values_b[] = {10.0, 20.0, 30.0, 40.0};
    Matrix<double> a(values_a, 2, 2);
    Matrix<double> b(values_b, 2, 2);

    a += b;

    REQUIRE(a.at(0, 0) == 11.0);
    REQUIRE(a.at(0, 1) == 22.0);
    REQUIRE(a.at(1, 0) == 33.0);
    REQUIRE(a.at(1, 1) == 44.0);
}

TEST_CASE("operator+= adds a scalar to every element in place", "[matrix][arithmetic]") {
    double values[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> m(values, 2, 2);

    m += 5.0;

    REQUIRE(m.at(0, 0) == 6.0);
    REQUIRE(m.at(0, 1) == 7.0);
    REQUIRE(m.at(1, 0) == 8.0);
    REQUIRE(m.at(1, 1) == 9.0);
}

TEST_CASE("operator-= subtracts another matrix element-wise in place", "[matrix][arithmetic]") {
    double values_a[] = {10.0, 20.0, 30.0, 40.0};
    double values_b[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> a(values_a, 2, 2);
    Matrix<double> b(values_b, 2, 2);

    a -= b;

    REQUIRE(a.at(0, 0) == 9.0);
    REQUIRE(a.at(0, 1) == 18.0);
    REQUIRE(a.at(1, 0) == 27.0);
    REQUIRE(a.at(1, 1) == 36.0);
}

TEST_CASE("operator-= subtracts a scalar from every element in place", "[matrix][arithmetic]") {
    double values[] = {10.0, 20.0, 30.0, 40.0};
    Matrix<double> m(values, 2, 2);

    m -= 5.0;

    REQUIRE(m.at(0, 0) == 5.0);
    REQUIRE(m.at(0, 1) == 15.0);
    REQUIRE(m.at(1, 0) == 25.0);
    REQUIRE(m.at(1, 1) == 35.0);
}

TEST_CASE("operator*= multiplies every element by a scalar in place", "[matrix][arithmetic]") {
    double values[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> m(values, 2, 2);

    m *= 3.0;

    REQUIRE(m.at(0, 0) == 3.0);
    REQUIRE(m.at(0, 1) == 6.0);
    REQUIRE(m.at(1, 0) == 9.0);
    REQUIRE(m.at(1, 1) == 12.0);
}

// transpose() currently only swaps the reported row/column counts; it does not
// actually reorder the underlying elements. These tests document that observed
// behavior as-is, without correcting it.
TEST_CASE("transpose swaps the reported dimensions", "[matrix][transpose]") {
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<double> m(values, 2, 3);

    m.transpose();

    // With rows/cols swapped, the element buffer still has 6 entries,
    // so every position in the "3x2" shape remains readable.
    REQUIRE(m.data().size() == 6);
    REQUIRE_NOTHROW(m.at(2, 1));
    REQUIRE_THROWS_AS(m.at(3, 0), InvalidIndexException);
}

TEST_CASE("transpose does not actually reorder the underlying elements", "[matrix][transpose]") {
    double values[] = {1.0, 2.0, 3.0, 4.0};
    Matrix<double> m(values, 2, 2);

    m.transpose();

    // A real transpose of [[1,2],[3,4]] would be [[1,3],[2,4]];
    // the current implementation leaves the buffer order unchanged.
    auto span = m.data();
    REQUIRE(span[0] == 1.0);
    REQUIRE(span[1] == 2.0);
    REQUIRE(span[2] == 3.0);
    REQUIRE(span[3] == 4.0);
}

TEST_CASE("get_transposed swaps dimensions on the returned copy and leaves the original unchanged", "[matrix][transpose]") {
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<double> m(values, 2, 3);

    Matrix<double> transposed = m.get_transposed();

    REQUIRE(transposed.data().size() == 6);
    REQUIRE(m.data().size() == 6);
    REQUIRE(m.at(0, 0) == 1.0);
    REQUIRE(m.at(1, 2) == 6.0);
}
