#include <iostream>
#include "polygon.h"
#include "shape.h"
#include "drawing.h"

void Polygon::draw(SDL_Renderer* renderer) const {
    Shape::draw(renderer);
}
