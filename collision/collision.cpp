#include "collision.h"
#include "../graphics/circle.h"
#include "../vector/vector2.h"

// Initially this class returned a bool, now it will return a manifold
std::unique_ptr<Manifold> Collision::checkCircleCircle(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();
	Vector2 direction = centroidB - centroidA;
	float directionLength = direction.length();

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();
	float sumRadius = radiusA + radiusB;

	if (directionLength < sumRadius) {
		// Normalizing with scale to get a new vector
		Vector2 penetrationNormal = Scale(direction, 1 / directionLength); 
		float penetrationDepth = directionLength - sumRadius; 
		Vector2 penetrationPoint = centroidA + Scale(penetrationNormal, radiusA);

		// return new Manifold(penetrationDepth * -1, penetrationNormal, penetrationPoint);
		return std::make_unique<Manifold>(penetrationDepth * -1, penetrationNormal, penetrationPoint);
	}

	return nullptr;
}

float Collision::calculatePenetrationDepthOfCircles(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();

	Vector2 direction = centroidB - centroidA;

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();

	return radiusA + radiusB - direction.length();
}