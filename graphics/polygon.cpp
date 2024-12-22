#include <iostream>
#include "polygon.h"
#include "shape.h"
#include "drawing.h"

void Polygon::draw(SDL_Renderer* renderer) const {
    Shape::draw(renderer);
    for (size_t i = 0; i < normals.size(); ++i) {
        // Vector2 start = (vertices[i] + vertices[(i + 1) % vertices.size()]);
        Vector2 direction = vertices[(i + 1) % vertices.size()] - vertices[i];
        Vector2 center = vertices[i] + Scale(direction, .5);
        Vector2 end = center + Scale(normals[i], 15);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);  // White color
        SDL_RenderDrawLine(renderer, center.getX(), center.getY(), end.getX(), end.getY());
    }
}

const std::vector<Vector2>& Polygon::getNormals() const {
    return normals;
}

/* Vertices must be listed counterclockwise. */
std::vector<Vector2> Polygon::calculateNormals() const {
    std::vector<Vector2> normals;
    for (size_t i = 0; i < vertices.size(); i++) {
        Vector2 direction = vertices[(i + 1) % vertices.size()] - vertices[i];
        direction.normalize();
        Vector2 normal = direction.getNormal();
        normals.push_back(normal);
    }

    return normals;
}

void Polygon::rotate(float radiansDelta) {
    Shape::rotate(radiansDelta);
    normals = calculateNormals();
}
