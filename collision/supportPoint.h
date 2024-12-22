#ifndef _support_point_h
#define _support_point_h

#include <SDL.h>
#include "../vector/vector2.h"

class SupportPoint {
protected:
    Vector2 penetrationPoint;
    float depth;
    
public:
    // static const SupportPoint Null; // Declare Null

    // SupportPoint() : penetrationPoint(Vector2::Null), depth(0.0f) {}

    SupportPoint(Vector2 penetrationPoint, float depth) :
        penetrationPoint(penetrationPoint), depth(depth) {};
    
    virtual ~SupportPoint() = default;  

    // bool isNull() const; 

    float getDepth() const {
        return depth;
    }

    Vector2 getPenetrationPoint() const {
        return penetrationPoint;
    }

    friend std::ostream& operator<<(std::ostream& os, const SupportPoint& supportPoint);

};

#endif