/**
 * File: rectangle.h 
 * -----------------
 * This interface exports the Rectangle class, which represents a 
 * rectangle in the x-y plane.
 */

#ifndef _rectangle_h
#define _rectangle_h

#include <iostream>
#include <vector>
#include <SDL.h>
#include "../vector/vector2.h"
#include "shape.h"
#include "polygon.h"

class Rectangle : public Polygon {
public:
    // Rectangle(const Vector2& position, int width, int height, Color color);
    Rectangle(const Vector2& position, int width, int height, SDL_Color color);

    float calculateInertia(float mass) override {
        return mass;
    }
};

#endif