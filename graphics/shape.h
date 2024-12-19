/**
 * File: shape.h 
 * --------------
 * This interface exports the Shape class, which represents a 
 * shape in the x-y plane.
 */

#ifndef _shape_h
#define _shape_h

#include <iostream>
#include <SDL.h>
#include "../vector/vector2.h"
#include "helper.h"

class Shape {
protected:
    std::vector<Vector2> vertices; // A collection of vertices
    Vector2 centroid;
    Color color;
    
public:
    Shape(const std::vector<Vector2>& verticesC) : vertices(verticesC) {
        centroid = calculateCentroid();
        Color newColor = {255, 255, 255};
        setColor(newColor);
    };
    virtual ~Shape() = default;  

    // getters
    virtual Vector2 getCentroid() const; 
    virtual const char* getType() const = 0;
    const std::vector<Vector2>& getVertices() const;
    Color const getColor() const;

    // setters
    virtual void setCentroid(const Vector2& newCentroid);
    void setColor(Color newColor);

    // manipulation and calculation
    Vector2 calculateCentroid() const;
    float calculateArea() const;
    const Vector2 rotateAroundPoint(Vector2 vec, Vector2 point, float angle) const;
    virtual void move(Vector2 delta);
    void rotate(float radiansDelta);


    // drawing
    virtual void draw(SDL_Renderer* renderer) const;
};

#endif