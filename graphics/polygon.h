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

// Derived class: Circle
class Polygon : public Shape {
private:

public:
     // Constructor
    Polygon();
    Polygon(SDL_Renderer* rendererC, const std::vector<Vector2>& verticesC);

    // Draw method implementation
    void draw() const override;

    const std::vector<Vector2>& getVertices() const;
};

#endif