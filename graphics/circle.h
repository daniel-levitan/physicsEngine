/**
 * File: circle.h 
 * --------------
 * This interface exports the Circle class, which represents a 
 * circle in the x-y plane.
 */

#ifndef _circle_h
#define _circle_h

#include <iostream>
#include <SDL.h>
#include "shape.h"
#include "../vector/vector2.h"
#include "../collision/manifold.h"
#include "../collision/collision.h"


class Circle : public Shape {
private:
    Vector2 center;
    float radius;

public:
    // Circle(const Vector2& center, int radius, Color color)
    Circle(const Vector2& center, int radius, SDL_Color color)
        : Shape({ center, Vector2(center.getX() + radius, center.getY()) }, color), center(center), radius(radius) 
    {
        // initialCentroid = centroid = center;
        setCentroid(center);
        initialCentroid = center;
    }

    void draw(SDL_Renderer* renderer) const override;

    const float getRadius() const;

    // void setCentroid(const Vector2& newCentroid); 

    void move(Vector2 delta) override;

    void resetPosition() override;

    std::unique_ptr<Manifold> acceptCollision(Shape& other) override {
        return other.collideWith(*this);
    }

    std::unique_ptr<Manifold> collideWith(Circle& circle) override {
        return Collision::checkCircleCircle(*this, circle);
    }

    std::unique_ptr<Manifold> collideWith(Polygon& polygon) override {
        return Collision::checkCirclePolygon(*this, polygon);        
    }

    float calculateInertia(float mass) override {
        return 0.5 * mass * radius * radius;
    }
};
#endif