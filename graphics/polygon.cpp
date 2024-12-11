#include <iostream>
#include "polygon.h"
#include "shape.h"


// Constructor
Polygon::Polygon() : Shape() {}

Polygon::Polygon(SDL_Renderer* rendererC, const std::vector<Vector2>& verticesC) :
    Shape(rendererC, verticesC) {}


// Draw method
void Polygon::draw() const {
    Shape::draw();
}

const std::vector<Vector2>& Polygon::getVertices() const {
    return Shape::getVertices();
}
