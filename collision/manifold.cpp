#include <iostream>
#include "manifold.h"
#include "../vector/vector2.h"
#include "../graphics/graphics.h"
#include "../graphics/helper.h"
#include "../graphics/drawing.h"
#include "../utils/utils.h"

const float Manifold::getDepth() const {
	return depth;
}
    
const Vector2 Manifold::getNormal() const {
	return normal;
}

const Vector2 Manifold::getPenetrationPoint() const {
	return penetrationPoint;
}

// bool Manifold::isNull() const {
//     return depth == 0.0f && normal == Vector2::Null && penetrationPoint == Vector2::Null;        
// }

void Manifold::draw(SDL_Renderer* renderer) const {
    const Vector2& startPoint = penetrationPoint + Scale(normal, depth * -1);
    Drawing::drawArrow(renderer, startPoint, penetrationPoint);        
}

std::string Manifold::toString() {
    std::string normalStr = "(" + Utils::to_string_with_precision(normal.getX(),1) + ", " +
        Utils::to_string_with_precision(normal.getY(),1) + ")";

    std::string penPointStr = "(" + Utils::to_string_with_precision(penetrationPoint.getX(),2) + ", " +
        Utils::to_string_with_precision(penetrationPoint.getY(),2) + ")";


    std::string result = "Depth: " + Utils::to_string_with_precision(depth, 2) + "\n";
    result += "Normal: " + normalStr + "\n";
    result += "Penetration Point: " + penPointStr;

    return result; 
}

std::ostream& operator<<(std::ostream& os, const Manifold& manifold) {
    os << "Depth:" << manifold.depth;
    os << ", Normal:" << manifold.normal;
    os << ", Penetration Point:" << manifold.penetrationPoint;
    return os;
}


// Manifold::resolveCollision()
// Manifold::positionalCorrection()
