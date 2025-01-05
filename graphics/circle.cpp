#include <SDL.h>
#include "circle.h"
#include "shape.h"
#include "drawing.h"

void Circle::draw(SDL_Renderer* renderer) const {
    Shape::draw(renderer);

    // SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    
    if (overlap) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    } else {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    }

    for (int angle = 0; angle < 360; angle++) {
        float rad = angle * (M_PI / 180);  // Convert angle to radians
        int x = center.getX() + radius * cos(rad);
        int y = center.getY() + radius * sin(rad);
        SDL_RenderDrawPoint(renderer, x, y);  // Draw a point at (x, y)
    }

    // DrawFilledCircle(renderer, centroid.getX(), centroid.getY(), 2);
}

void Circle::resetPosition() {
    Shape::resetPosition();
    center = initialCentroid;
}

const float Circle::getRadius() const {
    return radius;
}

void Circle::setCentroid(const Vector2& newCentroid) {
    centroid = newCentroid;
} 

void Circle::move(Vector2 delta) {
    Shape::move(delta);
    center.setX(center.getX() + delta.getX());
    center.setY(center.getY() + delta.getY());
}