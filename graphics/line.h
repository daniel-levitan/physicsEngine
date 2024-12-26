#ifndef _line_h
#define _line_h

#include <iostream>
#include "shape.h"
#include "../vector/vector2.h"

class Line : public Shape {
private:

public:
    Line(const Vector2 start, const Vector2 end, Color color, std::string type) : Shape({start, end}, color, type) {}

    void draw(SDL_Renderer* renderer) const override;
};

#endif