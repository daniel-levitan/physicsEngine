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

class Polygon : public Shape {
private:
     std::vector<Vector2> normals;

public:
     Polygon(const std::vector<Vector2>& vertices, Color color, std::string type) : Shape(vertices, color, type) {
          normals = calculateNormals();
     }

     const std::vector<Vector2>& getNormals() const;

     void draw(SDL_Renderer* renderer) const override;

     void rotate(float radiansDelta) override;

     std::vector<Vector2> calculateNormals() const;
};

#endif