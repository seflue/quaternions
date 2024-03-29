#include "quaternion.h"
#include "xyz.h"
#include <cmath>
#include <string>

namespace q = quaternions;

q::quaternion q::quaternion::from_vector(const xyz& v) {
    return quaternion{0, v.x, v.y, v.z};
}

q::quaternion q::quaternion::from_rotation(q::rotation r) {
    return quaternion{
        cos(r.angle / 2.),
        r.axis.x * sin(r.angle / 2.),
        r.axis.y * sin(r.angle / 2.),
        r.axis.z * sin(r.angle / 2.)
    };
}

std::string q::quaternion::to_string() const
{
    return std::string("{ ") +
        "w: " + std::to_string(w) + ", " +
        "x: " + std::to_string(x) + ", " +
        "y: " + std::to_string(y) + ", " +
        "z: " + std::to_string(z) +
        " }";
}

q::xyz q::quaternion::vector() const
{
    return xyz{x, y, z};
}

double q::quaternion::scalar() const
{
    return w;
}

q::rotation quaternions::quaternion::rotation() const {
    auto const divisor = sqrt(1 - w * w);
    return q::rotation{
        xyz{
            (x / divisor),
            (y / divisor),
            (z / divisor)
        },
        2 * acos(w)
    };
}

q::quaternion q::quaternion::inverted() const {
    const auto amplitude_squared = w * w + x * x + y * y + z * z;
    const auto conj = conjugated();
    return quaternion{
        conj.w / amplitude_squared,
        conj.x / amplitude_squared,
        conj.y / amplitude_squared,
        conj.z / amplitude_squared
    };
}

q::quaternion q::quaternion::conjugated() const {
    return q::quaternion{w, -x, -y, -z};
}

q::quaternion q::quaternion::operator-() const {
    return q::quaternion{-w, -x, -y, -z};
}

q::quaternion q::quaternion::cross(const quaternion& other) const {
    return quaternion{
            0,
            this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x,
    };
}

double q::quaternion::dot(const quaternion &other) const {
    return
            this->w * other.w +
            this->x * other.x +
            this->y * other.y +
            this->z * other.z;
}

double q::quaternion::norm() const {
    return dot(*this);
}

double q::quaternion::length() const {
    return sqrt(norm());
}

q::quaternion q::quaternion::normalized() const {
    const auto t_norm = length();
    return quaternion{
            w / t_norm,
            x / t_norm,
            y / t_norm,
            z / t_norm
    };
}

q::quaternion q::quaternion::polar_direction() const {
    const auto diff = *this - conjugated();
    const auto length = diff.length();
    return q::quaternion{
        0,
        diff.x / length,
        diff.y / length,
        diff.z / length,
    };
}

double q::quaternion::polar_angle() const {
    const auto scalar_val = (*this + conjugated()).scalar();
    return acos(scalar_val/(2*length()));
}

std::optional<q::quaternion> q::quaternion::rotated(const q::quaternion& r) const {
    if(!almost_equal(r.length(), 1.0))
        return std::nullopt;
    return r * *this * r.conjugated();
}

q::matrix_3x3 q::quaternion::to_matrix() const {
    return {
        {1 - 2 * (y * y + z * z), 2 * (x * y + z * w), 2 * (x * z - y * w)},
        {2 * (x * y - z * w), 1 - 2 * (x * x + z * z), 2 * (y * z + x * w)},
        {2 * (x * z + y * w), 2 * (y * z - x * w), 1 - 2 * (x * x + y * y)},
    };
}

q::quaternion q::operator+(const quaternion& a, const quaternion& b)
{
    return q::quaternion{a.w + b.w, a.x + b.x, a.y + b.y, a.z + b.z};
}

q::quaternion q::operator+(const quaternion &q, const double s) {
    return q + q::quaternion{s, 0, 0, 0};
}

q::quaternion q::operator+(const double s, const quaternion& q) {
    return q + s;
}

q::quaternion q::operator-(const quaternion& a, const quaternion& b)
{
    return q::quaternion{a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z};
}

q::quaternion q::operator*(const quaternion& a, const quaternion& b) {
    return q::quaternion{
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    };
}

q::quaternion q::operator*(const quaternion& q, const double s) {
    return q::quaternion{
        q.w * s,
        q.x * s,
        q.y * s,
        q.z * s,
    };
}

q::quaternion q::operator*(const double s, const quaternion& q) {
    return q * s;
}

bool q::operator==(const quaternion& a, const quaternion& b) {
    return a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z;
}

bool q::almost_equal(const q::quaternion& a, const q::quaternion& b, double eps) {
    return
        almost_equal(a.w, b.w, eps) &&
        almost_equal(a.x, b.x, eps) &&
        almost_equal(a.y, b.y, eps) &&
        almost_equal(a.z, b.z, eps);
}
