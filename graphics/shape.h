#ifndef _shape_h
#define _shape_h

#include <iostream>
#include <SDL.h>
#include "../vector/vector2.h"
#include "helper.h"
#include "../collision/manifold.h"
#include "text.h"

class Circle;  // Forward declaration
class Polygon; // Forward declaration

class Shape {
protected:
    std::vector<Vector2> vertices; // A collection of vertices
    Vector2 centroid;
    // Color color;
    SDL_Color color;
    float angle;
    bool overlap;
    
    std::vector<Vector2> initialVertices;
    Vector2 initialCentroid;
    float initialAngle;

    static int instances;

public:
    
    Text *text;

    Shape(const std::vector<Vector2>& verticesC, SDL_Color colorC) : 
        vertices(verticesC), color(colorC) {
        ++instances;
        angle = initialAngle = 0;
        if (vertices.size() > 2) {
            centroid = initialCentroid = calculateCentroid();
            const std::string message = std::to_string(instances);
            text = new Text("../assets/fonts/Arial-Unicode.ttf", message, 10, centroid, color);
        }

        overlap = false;

        initialVertices.assign(vertices.begin(), vertices.end());
    };

    virtual ~Shape() {
        --instances;
        if (text)
            delete text;
    } //= default;  

    // getters
    virtual Vector2 getCentroid() const; 
    const std::vector<Vector2>& getVertices() const;
    SDL_Color getColor();
    float getAngle();
    bool getOverlap();
    
    // setters
    void setCentroid(const Vector2& newCentroid);
    void setColor(SDL_Color newColor);
    virtual void resetPosition();
    void setOverlap(bool flag);

    // manipulation and calculation
    virtual std::unique_ptr<Manifold> acceptCollision(Shape& other) = 0;
    virtual std::unique_ptr<Manifold> collideWith(Circle& circle) = 0;
    virtual std::unique_ptr<Manifold> collideWith(Polygon& polygon) = 0;
    
    virtual float calculateInertia(float mass) = 0;

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