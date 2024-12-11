/**
 * File: rectangle.h 
 * -----------------
 * This interface exports the Rectangle class, which represents a 
 * rectangle in the x-y plane.
 */

#ifndef _rectangle_h
#define _rectangle_h

#include <iostream>
#include "polygon.h"
#include "../vector/vector2.h"

// Derived class: Rectangle
class Rectangle : public Polygon {
private:
    Vector2 position;
    float width;
    float height;    

public:
     // Constructor
    Rectangle();
    Rectangle(SDL_Renderer* rendererC, const Vector2& positionC, float widthC, float heightC);

    // Draw method implementation
    void draw() const override;

    // Getters for center and radius
    Vector2 getPosition() const;
    float getWidth() const;
    float getHeight() const;
};

#endif