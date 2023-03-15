#ifndef QUATERNIONS_QUATERNION_H
#define QUATERNIONS_QUATERNION_H
#include <optional>
#include <string>
#include "xyz.h"

namespace quaternions {
    struct rotation {
        xyz axis;
        double angle;
    };

    class quaternion {
    public:
        double w;
        double x;
        double y;
        double z;
        quaternion static from_vector(const xyz& v);
        quaternion static from_rotation(rotation r);
        std::string to_string() const;
        xyz vector() const;
        double scalar() const;
        rotation rotation() const;
        quaternion inverted() const;
        quaternion conjugated() const;
        quaternion operator-() const;
        quaternion cross(const quaternion& other) const;
        double dot(const quaternion& other) const;
        double norm() const;
        double length() const;
        quaternion normalized() const;
        quaternion polar_direction() const;
        double polar_angle() const;
        std::optional<quaternion> rotated(const quaternion& r) const;
    };

    quaternion operator+(const quaternion& a, const quaternion& b);
    quaternion operator+(const quaternion& q, const double s);
    quaternion operator+(const double s, const quaternion& q);
    quaternion operator-(const quaternion& a, const quaternion& b);
    quaternion operator*(const quaternion& a, const quaternion& b);
    quaternion operator*(const quaternion& q, const double s);
    quaternion operator*(const double s, const quaternion& q);

    bool operator==(const quaternion& a, const quaternion& b);
    bool almost_equal(double a, double b, double eps = 1E-12);
    bool almost_equal(const quaternion& a, const quaternion& b, double eps = 1E-12);
}

#endif //QUATERNIONS_QUATERNION_H
