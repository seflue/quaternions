#ifndef QUATERNIONS_TEST_HELPERS_H
#define QUATERNIONS_TEST_HELPERS_H
#include <catch2/matchers/catch_matchers_templated.hpp>
#include "../quaternions/xyz.h"
#include "../quaternions/quaternion.h"

namespace q = quaternions;

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

auto WithinAbs(const q::quaternion& quaternion, double eps = 1E-12) -> WithinAbsQuaternionMatcher;

namespace Catch{
    template<>
    struct StringMaker<q::xyz> {
        static std::string convert(const q::xyz& v) {
            return v.to_string();
        }
    };
}

struct WithinAbsXyzMatcher : Catch::Matchers::MatcherGenericBase {
    WithinAbsXyzMatcher(q::xyz const& xyz, double eps)
            : xyz{xyz}, eps{eps} {}

    template<typename OtherXyz>
    bool match(OtherXyz const& other) const {
        return q::almost_equal(xyz, other, eps);
    }

    std::string describe() const override {
        return "Equals: " + xyz.to_string();
    }

private:
    const q::xyz& xyz;
    const double eps;
};

auto WithinAbs(const q::xyz& xyz, double eps = 1E-12) -> WithinAbsXyzMatcher;

#endif //QUATERNIONS_TEST_HELPERS_H
