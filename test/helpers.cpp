#include "helpers.h"

auto WithinAbs(const q::xyz &xyz, double eps) -> WithinAbsXyzMatcher {
    return WithinAbsXyzMatcher{xyz, eps};
}

auto WithinAbs(const q::quaternion &quaternion,
               double eps) -> WithinAbsQuaternionMatcher {
    return WithinAbsQuaternionMatcher{quaternion, eps};
}
