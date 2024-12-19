#include "manifold.h"
#include "../vector/vector2.h"
#include "../graphics/graphics.h"
#include "../graphics/helper.h"
#include "../graphics/drawing.h"

const float Manifold::getDepth() const {
	return depth;
}
    
const Vector2 Manifold::getNormal() const {
	return normal;
}

const Vector2 Manifold::getPenetrationPoint() const {
	return penetrationPoint;
}

void Manifold::draw(SDL_Renderer* renderer) const {
	// Color color = {255, 255, 255}; // White color
    const Vector2& startPoint = penetrationPoint + Scale(normal, depth * -1);
    Drawing::drawArrow(renderer, startPoint, penetrationPoint);        
}

// Manifold::resolveCollision()
// Manifold::positionalCorrection()
