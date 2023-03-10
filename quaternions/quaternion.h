#ifndef QUATERNIONS_QUATERNION_H
#define QUATERNIONS_QUATERNION_H
#include <optional>

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
        std::optional<xyz> to_xyz() const;
        rotation to_rotation() const;
        quaternion conjugated() const;
        quaternion cross(const quaternion& other) const;
        double dot(const quaternion& other) const;
        double norm() const;
    };

    quaternion operator+(const quaternion& a, const quaternion& b);
    quaternion operator-(const quaternion& a, const quaternion& b);
    quaternion operator*(const quaternion& a, const quaternion& b);
}

#endif //QUATERNIONS_QUATERNION_H
