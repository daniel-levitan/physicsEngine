#ifndef _polygonFactory_h
#define _polygonFactory_h

#include <iostream>
#include "shape.h"
#include "polygon.h"

class PolygonFactory : public Polygon {

public:
    static std::unique_ptr<Polygon> createPolygon(int numOfSides, Vector2 center, float radius, Color color);

};

#endif
