#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include "quaternion.h"
#include <cmath>

namespace q = quaternions;
using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

namespace Catch{
    template<>
    struct StringMaker<q::quaternion> {
        static std::string convert(const q::quaternion& q) {
            return q.to_string();
        }
    };
}

struct WithinAbsQuaternionMatcher : Catch::Matchers::MatcherGenericBase {
    WithinAbsQuaternionMatcher(q::quaternion const& quaternion, double eps)
        : quaternion{quaternion}, eps{eps} {}

    template<typename OtherQuaternion>
    bool match(OtherQuaternion const& other) const {
        return q::almost_equal(quaternion, other, eps);
    }

    std::string describe() const override {
        return "Equals: " + quaternion.to_string();
    }

private:
    const q::quaternion& quaternion;
    const double eps;
};

auto WithinAbs(const q::quaternion& quaternion, double eps = 1E-12) -> WithinAbsQuaternionMatcher {
    return WithinAbsQuaternionMatcher{quaternion, eps};
}

TEST_CASE("initialize quaternion from vector")
{
    const auto q = q::quaternion::from_vector(q::xyz{1, 2, 3});
    const quaternions::xyz v = q.extract_vector();
    const auto s = q.extract_scalar();
    CHECK(s == 0);
    CHECK(v.x == 1);
    CHECK(v.y == 2);
    CHECK(v.z == 3);
}

TEST_CASE("initialize quaternion from rotation")
{
    const auto r = q::rotation{q::xyz{1, 0, 0}, M_PI_2};
    const auto q = q::quaternion::from_rotation(r);
    const auto r2 = q.to_rotation();

    CHECK_THAT(r2.axis.x, WithinRel(1, 1E-6));
    CHECK_THAT(r2.axis.y, WithinRel(0, 1E-6));
    CHECK_THAT(r2.axis.z, WithinRel(0, 1E-6));
    CHECK_THAT(r2.angle, WithinRel(M_PI_2));
}

TEST_CASE("add two quaternions")
{
    const auto qva = q::quaternion::from_vector({q::xyz{1, 0, 0}});
    const auto qvb = q::quaternion::from_vector({q::xyz{0, 1, 0}});
    const auto vab = (qva + qvb).to_xyz().value();
    CHECK_THAT(vab.x, WithinRel(1, 1E-6));
    CHECK_THAT(vab.y, WithinRel(1, 1E-6));
    CHECK_THAT(vab.z, WithinRel(0, 1E-6));
}

TEST_CASE("subtract two quaternions")
{
    const auto qva = q::quaternion::from_vector({q::xyz{3, 2, 1}});
    const auto qvb = q::quaternion::from_vector({q::xyz{1, 1, 1}});
    const auto vab = (qva - qvb).to_xyz().value();
    CHECK_THAT(vab.x, WithinRel(2, 1E-6));
    CHECK_THAT(vab.y, WithinRel(1, 1E-6));
    CHECK_THAT(vab.z, WithinRel(0, 1E-6));
}

TEST_CASE("multiply quaternion with scalar")
{
    const auto q = q::quaternion{1, 2, 3, 4};
    const auto s = 2.0;
    CHECK_THAT(q * s, WithinAbs(s * q));
    CHECK_THAT(q * s, WithinAbs(q::quaternion{2, 4, 6, 8}));
}

TEST_CASE("common multiplication (Grassmann product)")
{
    const auto qa = q::quaternion{0.1, 0.2, 0.3, 0.4};
    const auto qb = q::quaternion{0.2, 0.3, 0.4, 0.5};
    CHECK_THAT(qa * qb, WithinAbs(q::quaternion{-0.36, 0.06, 0.12, 0.12}));
}

TEST_CASE("cross product (Grassmann uneven product)")
{
    const auto x = q::xyz{1, 0, 0};
    const auto y = q::xyz{0, 1, 0};
    const auto qx = quaternions::quaternion::from_vector(x);
    const auto qy = quaternions::quaternion::from_vector(y);
    const auto qz = qx.cross(qy);
    const auto z2 = qz.to_xyz().value();
    CHECK_THAT(z2.x, WithinAbs(0, 1E-6));
    CHECK_THAT(z2.y, WithinAbs(0, 1E-6));
    CHECK_THAT(z2.z, WithinAbs(1, 1E-6));
}

TEST_CASE("dot product (euclidean inner product)") {
    const auto q = q::quaternion{1, 2, 3, 4};
    const auto qw = q::quaternion{1, 0, 0, 0};
    const auto qi = q::quaternion{0, 1, 0, 0};
    const auto qj = q::quaternion{0, 0, 1, 0};
    const auto qk = q::quaternion{0, 0, 0, 1};
    CHECK_THAT(q.dot(qw), WithinAbs(1, 1E-6));
    CHECK_THAT(q.dot(qi), WithinAbs(2, 1E-6));
    CHECK_THAT(q.dot(qj), WithinAbs(3, 1E-6));
    CHECK_THAT(q.dot(qk), WithinAbs(4, 1E-6));

    const auto qx = q::quaternion::from_vector(q::xyz{1, 0 , 0});
    const auto qy = q::quaternion::from_vector(q::xyz{0, 1 , 0});
    const auto qz = q::quaternion::from_vector(q::xyz{0, 0 , 1});
    CHECK_THAT(qx.dot(qy), WithinAbs(0, 1E-6));
    CHECK_THAT(qx.dot(qz), WithinAbs(0, 1E-6));
    CHECK_THAT(qy.dot(qz), WithinAbs(0, 1E-6));
}

TEST_CASE("inverse quaternion")
{
    const auto q = q::quaternion{1, 2, 3, 4};
    CHECK_THAT(q.inverse(), WithinAbs(q::quaternion{1/30.0, -1/15.0, -0.1, -2/15.0}));
    const auto qn = q.normalize();
    CHECK_THAT(qn.inverse(), WithinAbs(qn.conjugated()));
    CHECK_THAT(qn * qn.inverse(), WithinAbs(q::quaternion{1, 0, 0, 0}));
}

TEST_CASE("conjugate quaternion")
{
    const auto q = q::quaternion{1, 1, 1, 1};
    const auto conjugated = q.conjugated();

    CHECK(conjugated.w == 1);
    CHECK(conjugated.x == -1);
    CHECK(conjugated.y == -1);
    CHECK(conjugated.z == -1);
}

TEST_CASE("negate quaternion")
{
    const auto q = q::quaternion{1, 2, 3, 4};
    CHECK(-q == q::quaternion{-1, -2, -3, -4});
}

TEST_CASE("norm")
{
    const auto q = q::quaternion{1, 2, 3, 4};
    CHECK_THAT(q.norm(), WithinRel(1.0 + 4 + 9 + 16));
}

TEST_CASE("norm invariant")
{
    const auto q = q::quaternion{1, 2, 3, 4};
    CHECK_THAT(q.norm(), WithinRel((q.conjugated() * q).extract_scalar()));
}

TEST_CASE("length")
{
    const auto q1 = q::quaternion{1, 2, 3, 4};
    const auto q2 = q::quaternion{-1, -2, -3, -4};
    const auto q3 = q::quaternion{2, 3, 4, 5};
    const auto q4 = q::quaternion{2, -3, 4, -5};
    CHECK_THAT(q1.length(), WithinAbs(5.477225575051661, 1E-6));
    CHECK_THAT(q2.length(), WithinAbs(5.477225575051661, 1E-6));
    CHECK_THAT(q3.length(), WithinAbs(7.3484692283495345, 1E-6));
    CHECK_THAT(q4.length(), WithinAbs(7.3484692283495345, 1E-6));
}

TEST_CASE("normalization")
{
    const auto qv = q::quaternion{sqrt(3), 2,3,3};
    CHECK_THAT(qv.length(), WithinAbs(5, 1E-6));
    const auto qn = qv.normalize();
    CHECK_THAT(qn.length(), WithinAbs(1, 1E-6));
    CHECK_THAT(qn, WithinAbs(q::quaternion{0.34641016151377546, 0.4, 0.6, 0.6}));
}

TEST_CASE("rotating x 90 degrees around z should give y")
{
    const auto v = q::xyz{1, 0, 0};
    const auto r = q::rotation{q::xyz{0, 0, 1}, M_PI_2};
    const auto qv = q::quaternion::from_vector(v);
    const auto qr = q::quaternion::from_rotation(r);
    const auto q_rotated = qv.rotate(qr);
    CHECK(q_rotated != std::nullopt);
    CHECK_THAT(q_rotated.value(), WithinAbs(q::quaternion{0, 0, 1, 0}));
}

TEST_CASE("rotating z 90 degrees around x should give -y")
{
    const auto v = q::xyz{0, 0, 1};
    const auto r = q::rotation{q::xyz{1, 0, 0}, M_PI_2};
    const auto qv = q::quaternion::from_vector(v);
    const auto qr = q::quaternion::from_rotation(r);
    const auto q_rotated = qv.rotate(qr);
    CHECK(q_rotated != std::nullopt);
    CHECK_THAT(q_rotated.value(), WithinAbs(q::quaternion{0, 0, -1, 0}));
}

TEST_CASE("rotating y 90 degrees around x should give z")
{
    const auto v = q::xyz{0, 1, 0};
    const auto r = q::rotation{q::xyz{1, 0, 0}, M_PI_2};
    const auto qv = q::quaternion::from_vector(v);
    const auto qr = q::quaternion::from_rotation(r);
    const auto q_rotated = qv.rotate(qr);
    CHECK(q_rotated != std::nullopt);
    CHECK_THAT(q_rotated.value(), WithinAbs(q::quaternion{0, 0, 0, 1}));
}

TEST_CASE("not normalized rotation quaternion is not allowed")
{
    const auto v = q::xyz{1, 0, 0};
    const auto r = q::rotation{q::xyz{0, 0, 2}, M_PI_2};
    const auto qv = q::quaternion::from_vector(v);
    const auto qr = q::quaternion::from_rotation(r);
    const auto q_rotated = qv.rotate(qr);
    CHECK(q_rotated == std::nullopt);
}

TEST_CASE("direction of a quaternion in polar form")
{
    const auto q = q::quaternion{2, 2, 4, 4};
    const auto dir = q.polar_direction();
    CHECK_THAT(dir, WithinAbs(q::quaternion{0, 1/3.0, 2/3.0, 2/3.0}));
    CHECK_THAT(dir.length(), WithinRel(1.0));
    CHECK_THAT(dir * dir, WithinAbs(q::quaternion{-1, 0, 0, 0}));
}

TEST_CASE("angle of a quaternion in polar form")
{
    const auto q = q::quaternion{2, 2, 4, 4};
    const auto angle = q.polar_angle();
    CHECK_THAT(angle, WithinAbs(1.2490457724, 1E-6));
    const auto q_check =
        q.length() * (cos(angle) + (q.polar_direction() * sin(angle)));
    CHECK_THAT(q_check, WithinAbs(q));
}
