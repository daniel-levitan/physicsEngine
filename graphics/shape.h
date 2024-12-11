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
    SDL_Renderer* renderer;
	/**
     * Constructor: Shape
     * Usage: Shape shape(vector<Vector2> &vertices);     
     * ------------------------------
     * Creates a shape object.
  	 */
    // explicit Shape(const std::vector<Vector2>& vertices) : vertices(vertices) {}
    Shape();
    explicit Shape(SDL_Renderer* rendererC, const std::vector<Vector2>& verticesC);

    // Pure virtual method for drawing
	virtual void draw() const; 

    // Access vertices
    const std::vector<Vector2>& getVertices() const;

    // Virtual destructor
    virtual ~Shape();
};

#endif