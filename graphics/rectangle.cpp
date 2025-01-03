#include <iostream>
#include <SDL.h>
#include "rectangle.h"

// Rectangle::Rectangle(const Vector2& position, int width, int height, Color color, std::string type)
Rectangle::Rectangle(const Vector2& position, int width, int height, Color color)
   : Polygon({ 
        Vector2(position.getX() - width / 2, position.getY() - height / 2),
        Vector2(position.getX() + width / 2, position.getY() - height / 2),
        Vector2(position.getX() + width / 2, position.getY() + height / 2),
        Vector2(position.getX() - width / 2, position.getY() + height / 2)}, 
        color) {}
        // color, type) {}


// // std::unique_ptr<Manifold> acceptCollision(Shape& other) override {
// bool acceptCollision(Shape& other) override {
//    return other.collideWith(*this);
// }

// // std::unique_ptr<Manifold> collideWith(Circle& circle) override {
// bool collideWith(Circle& circle) override {
//    return checkCircleCircle(*this, circle);
// }

// // std::unique_ptr<Manifold> collideWith(Polygon& polygon) override {
// bool collideWith(Polygon& polygon) override {
//   return checkCirclePolygon(*this, polygon);
// }