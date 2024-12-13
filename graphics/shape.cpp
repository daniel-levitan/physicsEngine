/**
 * File: vector.cpp 
 * ----------------
 * This file implements the vector.h interface.
 */

#include <string>
#include <iostream>
#include <SDL.h>
#include "shape.h"

// Default draw implementation
void Shape::draw(SDL_Renderer* renderer) const {
        for (size_t i = 0; i < vertices.size(); ++i) {
            const Vector2& start = vertices[i];
            const Vector2& end = vertices[(i + 1) % vertices.size()]; // Loop back to the start
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
            SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
        }
    }

const std::vector<Vector2>& Shape::getVertices() const {
    return vertices;
}

Shape::~Shape() {
}

