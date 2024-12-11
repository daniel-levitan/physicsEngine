#include <iostream>
#include <SDL.h>
#include "rectangle.h"
#include "polygon.h"
#include "drawing.h"


// Constructor
Rectangle::Rectangle() : Polygon(), position(Vector2()), width(0), height(0) {}
Rectangle::Rectangle(SDL_Renderer* rendererC, const Vector2& positionC, float widthC, float heightC) :
    Polygon(rendererC, 
            {Vector2(positionC.getX() - widthC / 2, positionC.getY() - heightC / 2), 
             Vector2(positionC.getX() + widthC / 2, positionC.getY() - heightC / 2),
             Vector2(positionC.getX() + widthC / 2, positionC.getY() + heightC / 2),
             Vector2(positionC.getX() - widthC / 2, positionC.getY() + heightC / 2)}
            ),
    position(positionC),
    width(widthC),
    height(heightC) {}


// Draw method
void Rectangle::draw() const {
    Polygon::draw();    
}

float Rectangle::getWidth() const {
    return width;
}

float Rectangle::getHeight() const {
    return height;
}

Vector2 Rectangle::getPosition() const {
    return position;
}