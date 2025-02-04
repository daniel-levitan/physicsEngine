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

    virtual ~Manifold() = default;  

    const float getDepth() const;
    const Vector2 getNormal() const;
    const Vector2 getPenetrationPoint() const;

    void setNormal(Vector2 newNormal);
    void setDepth(float newDepth);

    void draw(SDL_Renderer* renderer) const;

    std::string toString();
    friend std::ostream& operator<<(std::ostream& os, const Manifold& manifold);
};
#endif