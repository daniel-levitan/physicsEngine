/**
 * File: polygon.h 
 * ---------------
 * This interface exports the Polygon class, which represents a 
 * polygon in the x-y plane.
 */

#ifndef _polygon_h
#define _polygon_h

#include <iostream>
#include "shape.h"
#include "../vector/vector2.h"

class Polygon : public Shape {
public:
     Polygon(const std::vector<Vector2>& vertices) : Shape(vertices) {}

     void draw(SDL_Renderer* renderer) const override;
};

#endif