/**
 * File: circle.h 
 * --------------
 * This interface exports the Circle class, which represents a 
 * circle in the x-y plane.
 */

#ifndef _circle_h
#define _circle_h

#include <iostream>
#include "shape.h"
#include "../vector/vector2.h"
#include "../collision/manifold.h"
#include "../collision/collision.h"

class Circle : public Shape {
private:
    Vector2 center;
    float radius;

public:
    Circle(const Vector2& center, int radius, Color color)
        : Shape({ center, Vector2(center.getX() + radius, center.getY()) }, color), center(center), radius(radius) {
            initialCentroid = centroid = center;
    }

    void draw(SDL_Renderer* renderer) const override;

    const float getRadius() const;

    void setCentroid(const Vector2& newCentroid) override; 

    void move(Vector2 delta) override;

    void resetPosition() override;

    float getDistanceFromCentroidToFloor() override { return center.getY() + radius; };

    std::unique_ptr<Manifold> acceptCollision(Shape& other) override {
    // bool acceptCollision(Shape& other) override {
        return other.collideWith(*this);
    }

    std::unique_ptr<Manifold> collideWith(Circle& circle) override {
    // bool collideWith(Circle& circle) override {
        // return Collision::checkCircleCircleBool(*this, circle);
        return Collision::checkCircleCircle(*this, circle);

    }

    std::unique_ptr<Manifold> collideWith(Polygon& polygon) override {
    // bool collideWith(Polygon& polygon) override {
        return Collision::checkCirclePolygon(*this, polygon);               
    }

    bool acceptFloorCollision(float floorXPosition) override {
        if (center.getY() + radius > floorXPosition)
            return true;
        return false;
    }
};
#endif