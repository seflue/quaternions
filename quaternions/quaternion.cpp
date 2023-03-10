#include "quaternion.h"
#include <cmath>

namespace q = quaternions;

q::quaternion q::quaternion::quaternion::from_vector(xyz v) {
    return quaternion{0, v.x, v.y, v.z};
}

q::quaternion q::quaternion::from_rotation(rotation r) {
    return quaternion{
        cos(r.angle / 2.),
        r.axis.x * sin(r.angle / 2.),
        r.axis.y * sin(r.angle / 2.),
        r.axis.z * sin(r.angle / 2.)
    };
}

std::optional<q::xyz> q::quaternion::to_xyz() const
{
    if(w != 0)
        return std::nullopt;
    else
        return xyz{x, y, z};
}

q::rotation quaternions::quaternion::to_rotation() const {
    return rotation{
        xyz{
                x / sqrt(1 - w * w),
                y / sqrt(1 - w * w),
                z / sqrt(1 - w * w)},
        2 * acos(w)
    };
}

q::quaternion q::quaternion::conjugated() const {
    return q::quaternion{w, -x, -y, -z};
}

q::quaternion q::operator+(const quaternion& a, const quaternion& b)
{
    return q::quaternion{a.w + b.w, a.x + b.x, a.y + b.y, a.z + b.z};
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
    return sqrt(w * w + x * x + y * y + z * z);
}