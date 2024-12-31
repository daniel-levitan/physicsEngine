#ifndef _collision_h
#define _collision_h

#include "../graphics/circle.h"
#include "../graphics/rectangle.h"
#include "manifold.h"
#include "supportPoint.h"

namespace Collision {
    // Circle vs Circle
    float calculatePenetrationDepthOfCircles(const Circle& c1, const Circle& c2);
    std::unique_ptr<Manifold> checkCircleCircle(const Circle& c1, const Circle& c2);
    // bool checkCircleCircle(const Circle& c1, const Circle& c2); // This was the first version

    // Polygon vs Polygon
    std::unique_ptr<SupportPoint> findSupportPoint(Vector2 normalOnEdge, Vector2 pointOnEdge, std::vector<Vector2> otherPolygonVertices);
    std::unique_ptr<Manifold> getContactPoint(const Polygon& pol1, const Polygon& pol2);
    std::unique_ptr<Manifold> checkPolygonPolygon(const Polygon& pol1, const Polygon& pol2);
    // bool checkPolygonPolygon(const Polygon& pol1, const Polygon& pol2);

    // New functions
    bool checkPolygonPolygonSAT(Polygon& pol1, Polygon& pol2);
    bool checkPolygonPolygonDIAG(Polygon& pol1, Polygon& pol2);
    bool resPolygonPolygonDIAG(Polygon& pol1, Polygon& pol2);
    std::unique_ptr<Manifold> resPolygonPolygonSAT(Polygon& pol1, Polygon& pol2);
    // float resPolygonPolygonSAT(Polygon& pol1, Polygon& pol2);

    void projectVertices(std::vector<Vector2>& vertices, Vector2& axis, float& min, float& max);
    bool intersectPolygons(Polygon& pol1, Polygon& pol2);
    std::unique_ptr<Manifold> resIntersectPolygons(Polygon& pol1, Polygon& pol2);
}

#endif