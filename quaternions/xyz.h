#ifndef QUATERNIONS_XYZ_H
#define QUATERNIONS_XYZ_H

#include <string>

namespace quaternions {
    struct xyz {
        double x;
        double y;
        double z;
        double norm() const;
        double length() const;
        bool is_normalized() const;
        xyz normalized() const;
        double dot(const xyz& other) const;
        std::string to_string() const;

        double operator[](uint32_t i) const;
    };

    xyz operator+(const xyz a, const xyz b);
    xyz operator*(const xyz a, const xyz b);
    xyz operator*(const double a, const xyz b);
    xyz operator*(const xyz a, const double b);

    bool almost_equal(double a, double b, double eps = 1E-12);
    bool almost_equal(const xyz& a, const xyz& b, double eps = 1E-12);

    /**
     * 3x3 matrix with column-major order
     */
    struct matrix_3x3 {
        xyz c1;
        xyz c2;
        xyz c3;

        xyz operator[](uint32_t i) const;
    };
}

#endif //QUATERNIONS_XYZ_H
