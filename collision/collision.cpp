#include "collision.h"
#include "../graphics/circle.h"
#include "../vector/vector2.h"
#include "supportPoint.h"


float Collision::calculatePenetrationDepthOfCircles(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();

	Vector2 direction = centroidB - centroidA;

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();

	return radiusA + radiusB - direction.length();
}

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

/**
 * If there is at least 1 vertice for which I cannot find a support point, the polygons
 * don't collide.
 */
std::unique_ptr<SupportPoint> Collision::findSupportPoint(Vector2 normal, Vector2 pointOnEdge, std::vector<Vector2> otherPolygonVertices) {
   	// float maxPenetrationDepth = -std::numeric_limits<float>::infinity();
   	float maxPenetrationDepth = 0;
	std::unique_ptr<SupportPoint> supportPoint = nullptr;

	// int i = 0;
    for (const auto& vertex : otherPolygonVertices) {
        Vector2 verticeToEdge = vertex - pointOnEdge;
        float depth = verticeToEdge.dotProduct(Scale(normal, -1));

        // std::cout << "Other polygon vertice " << vertex << " " << i++ << ": " << depth << std::endl;
        // Only updated when depth greater than 0
        if (depth > maxPenetrationDepth) {
            maxPenetrationDepth = depth;
            supportPoint = std::make_unique<SupportPoint>(vertex, depth);
        }  
    }

    return supportPoint;
}

/**
 * Looks for the best(deepest) contact point in which the two polygons touched.
 * */
std::unique_ptr<Manifold> Collision::getContactPoint(const Polygon& pol1, const Polygon& pol2) {
	float minPenetrationDepth = std::numeric_limits<float>::infinity();
	std::unique_ptr<Manifold> contact = nullptr;

    const auto& vertices = pol1.getVertices();
    const auto& normals = pol1.getNormals();

	for (size_t i = 0; i < normals.size(); i++) {
		const Vector2& pointOnEdge = vertices[i];
        const Vector2& normalOnEdge = normals[i];	

        // std::cout << "Normal(" << i << ") " << std::endl; 
		std::unique_ptr<SupportPoint> supportPoint = findSupportPoint(normalOnEdge, pointOnEdge, pol2.getVertices());		
		if (!supportPoint){		
			// std::cout << "Normal(" << i << ") " << "No support point" << std::endl;  
		    return nullptr;
		} else {
			// std::cout << "Support point(" << i << ") " << *supportPoint << std::endl;  
		}

		if (supportPoint->getDepth() < minPenetrationDepth) {
			minPenetrationDepth = supportPoint->getDepth();
			contact = std::make_unique<Manifold>(minPenetrationDepth, normalOnEdge, supportPoint->getPenetrationPoint());
		}
	}

	return contact;
}


std::unique_ptr<Manifold> Collision::checkPolygonPolygon(const Polygon& pol1, const Polygon& pol2) {
	std::unique_ptr<Manifold> contactPointPol1 = getContactPoint(pol1, pol2);	
	if (!contactPointPol1) 
		return nullptr;
		// return false;
	
	std::unique_ptr<Manifold> contactPointPol2 = getContactPoint(pol2, pol1);	
	if (!contactPointPol2) 
		return nullptr;
		// return false;
	
	if (contactPointPol1->getDepth() < contactPointPol2->getDepth())
		return std::make_unique<Manifold>(contactPointPol1->getDepth(), contactPointPol1->getNormal(), contactPointPol1->getPenetrationPoint());
	else
		return std::make_unique<Manifold>(contactPointPol2->getDepth(), contactPointPol2->getNormal(), contactPointPol2->getPenetrationPoint());
	// return true;
}



