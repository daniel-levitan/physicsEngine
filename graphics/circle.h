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

// Derived class: Circle
class Circle : public Shape {
private:
    Vector2 center;
    float radius;

public:
    Circle(const Vector2& center, int radius)
        : Shape({ center, Vector2(center.getX() + radius, center.getY()) }), center(center), radius(radius) {}

    void draw(SDL_Renderer* renderer) const override;

};

#endif