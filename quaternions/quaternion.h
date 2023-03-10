#ifndef QUATERNIONS_QUATERNION_H
#define QUATERNIONS_QUATERNION_H
#include <optional>
#include <string>

namespace quaternions {
    struct xyz {
        double x;
        double y;
        double z;
    };

    struct rotation {
        xyz axis;
        double angle;
    };

    class quaternion {
    public:
        const double w;
        const double x;
        const double y;
        const double z;
        quaternion static from_vector(xyz v);
        quaternion static from_rotation(rotation r);
        std::string to_string() const;
        std::optional<xyz> to_xyz() const;
        xyz extract_vector() const;
        double extract_scalar() const;
        rotation to_rotation() const;
        quaternion inverse() const;
        quaternion conjugated() const;
        quaternion cross(const quaternion& other) const;
        double dot(const quaternion& other) const;
        double norm() const;
        quaternion normalize() const;
    };

    quaternion operator+(const quaternion& a, const quaternion& b);
    quaternion operator-(const quaternion& a, const quaternion& b);
    quaternion operator*(const quaternion& a, const quaternion& b);
    quaternion operator*(const quaternion& q, const double s);
    quaternion operator*(const double s, const quaternion& q);

    bool almost_equal(double a, double b, double eps = 1E-12);
    bool almost_equal(const quaternion& a, const quaternion& b, double eps = 1E-12);
}

#endif //QUATERNIONS_QUATERNION_H
