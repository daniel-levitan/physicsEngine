/**
 * File: shape.h 
 * --------------
 * This interface exports the Shape class, which represents a 
 * shape in the x-y plane.
 */

#ifndef _shape_h
#define _shape_h

#include <iostream>
#include <SDL.h>
#include "../vector/vector2.h"

class Shape {
protected:
    std::vector<Vector2> vertices; // A collection of vertices
    
public:
    Shape(const std::vector<Vector2>& verticesC) : vertices(verticesC) {};

    virtual void draw(SDL_Renderer* renderer) const;    
    const std::vector<Vector2>& getVertices() const;

    virtual ~Shape(); // = default;  
};

#endif