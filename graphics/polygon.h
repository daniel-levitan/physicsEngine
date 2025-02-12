/**
 * File: polygon.h 
 * ---------------
 * This interface exports the Polygon class, which represents a 
 * polygon in the x-y plane.
 */

#ifndef _polygon_h
#define _polygon_h

#include <iostream>
#include <SDL.h>
#include "shape.h"
#include "../vector/vector2.h"
#include "../collision/manifold.h"
#include "../collision/collision.h"

class Polygon : public Shape {
private:
      std::vector<Vector2> normals;

public:
      // Polygon(const std::vector<Vector2>& vertices, Color color) : Shape(vertices, color) {
      Polygon(const std::vector<Vector2>& vertices, SDL_Color color) : Shape(vertices, color) {
         normals = calculateNormals();
      }

      const std::vector<Vector2>& getNormals() const;

      void draw(SDL_Renderer* renderer) const override;

      void rotate(float radiansDelta) override;

      std::vector<Vector2> calculateNormals() const;

      std::unique_ptr<Manifold> acceptCollision(Shape& other) override {
         return other.collideWith(*this);
      }

      std::unique_ptr<Manifold> collideWith(Circle& circle) override {
         return Collision::checkCirclePolygon(circle, *this);         
      }

      std::unique_ptr<Manifold> collideWith(Polygon& polygon) override {
         return Collision::resPolygonPolygonSAT(*this, polygon);
      }

      float calculateInertia(float mass) override {
         return mass;
    }
};

#endif