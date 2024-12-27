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
    std::string type;
    float angle;
    bool overlap;
    
    std::vector<Vector2> initialVertices;
    Vector2 initialCentroid;
    float initialAngle;

public:
    Shape(const std::vector<Vector2>& verticesC, Color colorC, std::string typeC) : 
        vertices(verticesC), color(colorC), type(typeC) {        
        angle = initialAngle = 0;
        centroid = initialCentroid = calculateCentroid();
        overlap = false;

        initialVertices.assign(vertices.begin(), vertices.end());
    };
    virtual ~Shape() = default;  

    // getters
    virtual Vector2 getCentroid() const; 
    const std::string getType() const;
    const std::vector<Vector2>& getVertices() const;
    Color getColor();
    float getAngle();
    bool getOverlap();

    // setters
    virtual void setCentroid(const Vector2& newCentroid);
    void setColor(Color newColor);
    void resetPosition();
    void setOverlap(bool flag);

    // manipulation and calculation
    Vector2 calculateCentroid() const;
    float calculateArea() const;
    const Vector2 rotateAroundPoint(Vector2 vec, Vector2 point, float angle) const;
    virtual void move(Vector2 delta);
    virtual void rotate(float radiansDelta);
    virtual void setVertices(std::vector<Vector2>& verticesC);


    // drawing
    virtual void draw(SDL_Renderer* renderer) const;
};

#endif