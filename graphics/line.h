#ifndef _line_h
#define _line_h

#include <iostream>
#include "shape.h"
#include "../vector/vector2.h"

class Line : public Shape {
private:

public:
    Line(const Vector2 start, const Vector2 end) : Shape({start, end}) {

    }

    void draw(SDL_Renderer* renderer) const override;
    
    // Override getType to return the specific type of shape
    const char* getType() const override {
        return "Line";
    }

};

#endif