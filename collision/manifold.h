#ifndef _manifold_h
#define _manifold_h

#include <SDL.h>
#include "../vector/vector2.h"
#include "../graphics/graphics.h"

class Manifold {
protected:
    float depth;
    Vector2 normal;
    Vector2 penetrationPoint;
    
public:
    Manifold(float depth, Vector2 normal, Vector2 penetrationPoint) :
        depth(depth), normal(normal), penetrationPoint(penetrationPoint) {};

    // Manifold() : depth(0.0f), normal(Vector2::Null), penetrationPoint(Vector2::Null) {}
    virtual ~Manifold() = default;  

    // bool isNull() const; 

    // getters
    // virtual Vector2 getCentroid() const; 
    const float getDepth() const;
    const Vector2 getNormal() const;
    const Vector2 getPenetrationPoint() const;

    // setters
    // virtual void setCentroid(const Vector2& newCentroid);
    // void setColor(Color newColor);

    // resolveCollision()
    // positionalCorrection()
    void draw(SDL_Renderer* renderer) const;

    friend std::ostream& operator<<(std::ostream& os, const Manifold& manifold);
};



#endif