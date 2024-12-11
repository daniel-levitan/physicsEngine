#include <iostream>
#include <SDL.h>
#include "circle.h"
#include "shape.h"
#include "drawing.h"


// Constructor
Circle::Circle() {
    Shape();
    radius = 0;
}

Circle::Circle(SDL_Renderer* rendererC, const Vector2& centerC, float radiusC) :
    Shape(rendererC, {centerC, Vector2(centerC.getX() + radiusC, centerC.getY())}),
    center(centerC),
    radius(radiusC) {}


// Draw method
void Circle::draw() const {
    Shape::draw();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    DrawCircle(renderer, center.getX(), center.getY(), radius);
}

// Get center
Vector2 Circle::getCenter() const {
    return center;
}

// Get radius
float Circle::getRadius() const {
    return radius;
}