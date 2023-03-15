#include "xyz.h"
#include <cmath>

namespace q = quaternions;

double quaternions::xyz::norm() const { return x*x + y*y + z*z; }

double quaternions::xyz::length() const { return std::sqrt(norm()); }

bool quaternions::xyz::is_normalized() const {
    return q::almost_equal(norm(), 1, 1E-6);
}

quaternions::xyz quaternions::xyz::normalized() const {
    return is_normalized()
           ? *this
           : [&](){
                const auto length = this->length();
                return xyz {
                        x/length,
                        y/length,
                        z/length,
                };
            }();
}

double quaternions::xyz::dot(const xyz& other) const {
    return x * other.x + y * other.y + z * other.z;
}

std::string q::xyz::to_string() const
{
    return std::string("{ ") +
           "x: " + std::to_string(x) + ", " +
           "y: " + std::to_string(y) + ", " +
           "z: " + std::to_string(z) +
           " }";
}

q::xyz q::operator+(const xyz a, const xyz b) {
    return {
            a.x + b.x,
            a.y + b.y,
            a.z + b.z,
    };
}

q::xyz q::operator*(const xyz a, const xyz b) {
    return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x,
    };
}

q::xyz q::operator*(const double a, const q::xyz b) {
    return {
            a * b.x,
            a * b.y,
            a * b.z,
    };
}

q::xyz q::operator*(const xyz a, const double b) {
    return b * a;
}

bool q::almost_equal(double a, double b, double eps) {
    return std::abs(a - b) < eps;
}

bool q::almost_equal(const q::xyz& a, const q::xyz& b, double eps) {
    return
            almost_equal(a.x, b.x, eps) &&
            almost_equal(a.y, b.y, eps) &&
            almost_equal(a.z, b.z, eps);
}

