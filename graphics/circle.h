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
     // Constructor
    Circle();
    Circle(SDL_Renderer* rendererC, const Vector2& centerC, float radiusC);

    // Draw method implementation
    void draw() const override;

    // Getters for center and radius
    Vector2 getCenter() const;
    float getRadius() const;
};

#endif