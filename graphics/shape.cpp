#include <string>
#include <iostream>
#include <cmath>
#include <SDL.h>
#include "shape.h"
#include "drawing.h"
#include "helper.h"

// Shape::~Shape() {
// }

const std::vector<Vector2>& Shape::getVertices() const {
    return vertices;
}

Vector2 Shape::getCentroid() const {
    return centroid;
}

Color Shape::getColor() {
    return color;
}

float Shape::getAngle() {
    return angle;
}

bool Shape::getOverlap() {
    return overlap;
}

void Shape::setCentroid(const Vector2& newCentroid) {
    centroid = newCentroid;    
}

void Shape::setColor(Color newColor) {
    color = newColor;
}

void Shape::setVertices(std::vector<Vector2>& verticesC) {
    vertices.clear();
    for (const auto& vertice : verticesC) {
        vertices.push_back(vertice);
    }
    centroid = calculateCentroid();
}

void Shape::setOverlap(bool flag) {
    overlap = flag;
}

void Shape::resetPosition() {
    vertices.clear();
    vertices.assign(initialVertices.begin(), initialVertices.end());
    centroid = initialCentroid;
    angle = initialAngle;
}

float Shape::calculateArea() const {
    float signedArea = 0.0f;

    for (size_t i = 0; i < vertices.size(); i++) {
        const Vector2& p1 = vertices[i];
        const Vector2& p2 = vertices[(i + 1) % vertices.size()];
        signedArea += p1.getX() * p2.getY() - p2.getX() * p1.getY();        
    }
    signedArea *= 0.5f;
    
    return signedArea;    
}

Vector2 Shape::calculateCentroid() const {
    Vector2 centroid(0, 0);
    float area = calculateArea();
    float Cx = 0;
    float Cy = 0;

    for (size_t i = 0; i < vertices.size(); i++) {
        const Vector2& p1 = vertices[i];
        const Vector2& p2 = vertices[(i + 1) % vertices.size()]; // To go back to the first element
        Cx += (p1.getX() + p2.getX()) * (p1.getX() * p2.getY() - p2.getX() * p1.getY());
        Cy += (p1.getY() + p2.getY()) * (p1.getX() * p2.getY() - p2.getX() * p1.getY());
    }
    centroid.setX(Cx / (6.0f * area));
    centroid.setY(Cy / (6.0f * area));

    return centroid;    
}

const Vector2 Shape::rotateAroundPoint(Vector2 vec, Vector2 point, float radians) const {
    Vector2 rotated(0, 0);

    Vector2 direction = vec - point;

    rotated.setX(direction.getX() * cos(radians) - direction.getY() * sin(radians));
    rotated.setY(direction.getX() * sin(radians) + direction.getY() * cos(radians));

    rotated = rotated + point;
    return rotated;
}

void Shape::move(Vector2 delta) {
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i] = vertices[i] + delta;
    }
    centroid = centroid + delta;
}

void Shape::rotate(float radiansDelta) {
    angle = radiansDelta;
    for (size_t i = 0; i < vertices.size(); i++) {
        Vector2 rotatedVertices = rotateAroundPoint(vertices[i], centroid, radiansDelta);         
        vertices[i] = rotatedVertices;
    }
}

// Default draw implementation
void Shape::draw(SDL_Renderer* renderer) const {
    // Selecting the color depending on state
    if (overlap) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    } else {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    }

    // Draw shape
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vector2& start = vertices[i];
        const Vector2& end = vertices[(i + 1) % vertices.size()]; // Loop back to the start
        SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
    }

    // Draw centroid
    Vector2 centroid = getCentroid();
    Drawing::DrawFilledCircle(renderer, centroid.getX(), centroid.getY(), 2);
    

    // Draw directional line
    Vector2 vertice = vertices[0]; 
    SDL_RenderDrawLine(renderer, centroid.getX(), centroid.getY(), vertice.getX(), vertice.getY());
}   