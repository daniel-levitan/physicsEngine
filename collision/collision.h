#ifndef _collision_h
#define _collision_h

#include "../graphics/circle.h"
#include "manifold.h"

namespace Collision {
    float calculatePenetrationDepthOfCircles(const Circle& c1, const Circle& c2);
    
    // bool checkCircleCircle(const Circle& c1, const Circle& c2);
    std::unique_ptr<Manifold> checkCircleCircle(const Circle& c1, const Circle& c2);

    // bool checkRectangleRectangle(const Rectangle& r1, const Rectangle& r2);
    // bool checkPolygonPolygon(const Polygon& p1, const Polygon& p2);
}

#endif