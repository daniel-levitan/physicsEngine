#include <iostream>
#include "supportPoint.h"

// const SupportPoint SupportPoint::Null(Vector2::Null, 0.0f);

// bool SupportPoint::isNull() const {
//         return penetrationPoint == Vector2::Null && depth == 0.0f;
//     }

std::ostream& operator<<(std::ostream& os, const SupportPoint& supportPoint) {
    os << "Penetration Point:" << supportPoint.penetrationPoint;
    os << ", Depth:" << supportPoint.depth;
    return os;
}