#include <iostream>
#include "polygon.h"
#include "shape.h"

void Polygon::draw(SDL_Renderer* renderer) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vector2& start = vertices[i];
        const Vector2& end = vertices[(i + 1) % vertices.size()]; // Loop back to the start
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
        SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
    }
}
