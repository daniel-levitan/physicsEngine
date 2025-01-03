#include "polygonFactory.h"
#include "polygon.h"
#include "../vector/vector2.h"
#include "helper.h"
#include "../engine/src/constants.h"
#include <math.h>

std::unique_ptr<Polygon> PolygonFactory::createPolygon(int numOfSides, Vector2 center, float radius, Color color) {
	float fTheta = M_PI * 2.0f / (float)numOfSides;	
	std::vector<Vector2> vertices;

	for (size_t i = 0; i < numOfSides; i++) 
		vertices.push_back(center + Vector2(radius * cos(fTheta * i), radius * sin(fTheta * i)));	
	
	return std::make_unique<Polygon>(vertices, color);	
}