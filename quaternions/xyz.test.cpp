#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "xyz.h"
#include <cmath>
#include <string>
#include "../test/helpers.h"

namespace q = quaternions;
using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

TEST_CASE("norm of a vector")
{
    const auto v = q::xyz{2, 3, 4};
    CHECK_THAT(v.norm(), WithinRel(29.0));
}

TEST_CASE("length of a vector")
{
    const auto v = q::xyz{2, 3, 6};
    CHECK_THAT(v.length(), WithinRel(7.0));
}

TEST_CASE("normalized vector")
{
    // TODO: Refactor with generator
    const auto v1 = q::xyz{5, 0, 0};
    const auto v2 = q::xyz{0, 0.2, 0};
    const auto v3 = q::xyz{0, 0, 120123};
    const auto vn1 = v1.normalized();
    const auto vn2 = v2.normalized();
    const auto vn3 = v3.normalized();
    CHECK_THAT(vn1.length(), WithinRel(1.));
    CHECK_THAT(vn2.length(), WithinRel(1.));
    CHECK_THAT(vn3.length(), WithinRel(1.));
    CHECK_THAT(vn1, WithinAbs(q::xyz{1, 0, 0}));
    CHECK_THAT(vn2, WithinAbs(q::xyz{0, 1, 0}));
    CHECK_THAT(vn3, WithinAbs(q::xyz{0, 0, 1}));
}

TEST_CASE("dot product")
{
    const auto v1 = q::xyz{2, 3, 4};
    const auto v2 = q::xyz{3, 4, 5};
    CHECK_THAT(v1.dot(v2), WithinRel(38.));
}

TEST_CASE("cross product")
{
    const auto v1 = q::xyz{2, 3, 4};
    const auto v2 = q::xyz{3, 4, 5};
    CHECK_THAT(v1 * v2, WithinAbs(q::xyz{-1, 2, -1}, 1E-6));
}