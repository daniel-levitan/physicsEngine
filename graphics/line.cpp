#include <iostream>
#include "line.h"
#include "shape.h"
#include "drawing.h"

void Line::draw(SDL_Renderer* renderer) const {
    Shape::draw(renderer);
}
