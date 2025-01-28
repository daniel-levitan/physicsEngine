/**
 * File: polygon.h 
 * ---------------
 * This interface exports the Polygon class, which represents a 
 * polygon in the x-y plane.
 */

#ifndef _polygon_h
#define _polygon_h

#include <iostream>
#include "shape.h"
#include "../vector/vector2.h"
#include "../collision/manifold.h"
#include "../collision/collision.h"

class Polygon : public Shape {
private:
      std::vector<Vector2> normals;

public:
      Polygon(const std::vector<Vector2>& vertices, Color color) : Shape(vertices, color) {
         normals = calculateNormals();
      }

      const std::vector<Vector2>& getNormals() const;

      void draw(SDL_Renderer* renderer) const override;

      void rotate(float radiansDelta) override;


      // The following function is WRONG. I will change it later.
      float getDistanceFromCentroidToFloor() override { return centroid.getY(); };

      std::vector<Vector2> calculateNormals() const;

      std::unique_ptr<Manifold> acceptCollision(Shape& other) override {
      // bool acceptCollision(Shape& other) override {
         return other.collideWith(*this);
      }

      std::unique_ptr<Manifold> collideWith(Circle& circle) override {
      // bool collideWith(Circle& circle) override {
         return Collision::checkCirclePolygon(circle, *this);
      }

      std::unique_ptr<Manifold> collideWith(Polygon& polygon) override {
      // bool collideWith(Polygon& polygon) override {
         // return Collision::intersectPolygons(*this, polygon);
         return Collision::checkPolygonPolygon(*this, polygon);
      }

      bool acceptFloorCollision(float floorXPosition) override {
         return false;
      }
};

#endif