#include <SDL.h>
#include "circle.h"
#include "shape.h"


void Circle::draw(SDL_Renderer* renderer) const {    
    Shape::draw(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for the circle
    for (int angle = 0; angle < 360; angle++) {
        float rad = angle * (M_PI / 180);  // Convert angle to radians
        int x = center.getX() + radius * cos(rad);
        int y = center.getY() + radius * sin(rad);
        SDL_RenderDrawPoint(renderer, x, y);  // Draw a point at (x, y)
    }

}