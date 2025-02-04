#include <iostream>
#include "manifold.h"
#include "../vector/vector2.h"
#include "../graphics/graphics.h"
#include "../graphics/helper.h"
#include "../graphics/drawing.h"
#include "../utils/utils.h"

const float Manifold::getDepth() const { return depth; }
    
const Vector2 Manifold::getNormal() const { return normal; }

void Manifold::setNormal(Vector2 newNormal) { normal = newNormal; }

void Manifold::setDepth(float newDepth) { depth = newDepth; }

const Vector2 Manifold::getPenetrationPoint() const {
	return penetrationPoint;
}

/** The normal vector is draw from a start point which is calculated from the
 *  contact point. Start Point = Contact Point - (Depth * -Normal)
 *  The minus indicate that we are going in the opposite direction of the 
 *  normal.
 * 
 *  The normal vector always points from a point within the shape towards 
 *  the contact point. It has magnitude of depth and its direction
 *  follows the normal.
 */
void Manifold::draw(SDL_Renderer* renderer) const {
    const Vector2& startPoint = Add(penetrationPoint, Scale(normal, depth * -1));
    Drawing::drawArrow(renderer, startPoint, penetrationPoint);
    Drawing::DrawFilledCircle(renderer, startPoint.getX(), startPoint.getY(), 3);
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