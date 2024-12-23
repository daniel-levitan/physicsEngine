#include <iostream>
#include <SDL.h>
#include "rectangle.h"

Rectangle::Rectangle(const Vector2& position, int width, int height, Color color)     
   : Polygon({ 
        Vector2(position.getX() - width / 2, position.getY() - height / 2),
        Vector2(position.getX() + width / 2, position.getY() - height / 2),
        Vector2(position.getX() + width / 2, position.getY() + height / 2),
        Vector2(position.getX() - width / 2, position.getY() + height / 2)
    }, color) {}
