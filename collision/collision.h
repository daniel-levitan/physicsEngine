#ifndef _collision_h
#define _collision_h

// #include "../graphics/circle.h"
// #include "../graphics/polygon.h"
// #include "../graphics/rectangle.h"
#include "manifold.h"
#include "supportPoint.h"
#include "../graphics/text.h"
#include "../rigid_body/rigid_body.h"

// #include "manifold.h"

class Shape;
class Circle;
class Polygon;
// class Rectangle;
// class Vector2;

namespace Collision {

    /**
     *  Functions related to simple shape movements 
     * */    
    // Circle vs Circle
    std::unique_ptr<Manifold> checkCircleCircle(const Circle& c1, const Circle& c2);

    // Polygon vs Polygon
    std::unique_ptr<Manifold> resPolygonPolygonSAT(Polygon& pol1, Polygon& pol2);

    // Circle vs Polygon
    std::unique_ptr<Manifold> checkCirclePolygonEdges(Circle& circ, Polygon& pol);
    std::unique_ptr<Manifold> checkCirclePolygonCorners(Circle& circ, Polygon& pol);
    std::unique_ptr<Manifold> checkCirclePolygon(Circle& circ, Polygon& pol);    

    // Double dispatch/visitor
    bool checkCollisionBool(Shape& s1, Shape& s2);
    std::unique_ptr<Manifold> checkCollision(Shape& s1, Shape& s2);

    /**
     *  END Functions related to simple shape movements 
     * */


    /**
     *  Functions related to rigid body 
     * */
    std::unique_ptr<Manifold> collisionDetection(RigidBody& rb1, RigidBody& rb2);
    void positionCorrection(RigidBody& rb1, RigidBody& rb2, Manifold& manifold);
    void resolveLinearCollisionOriginal(RigidBody& rb1, RigidBody& rb2, Manifold& manifold);
    void resolveLinearCollision(RigidBody& rb1, RigidBody& rb2, Manifold& manifold);
    // void resolveLinearCollision_v1(RigidBody& rb1, RigidBody& rb2, Manifold& manifold);

    void resolveCollision_v1(RigidBody& rb1, RigidBody& rb2, Manifold& manifold);
    void simplePositionCorrection(RigidBody& rb1, RigidBody& rb2, Manifold& manifold);
}

#endif