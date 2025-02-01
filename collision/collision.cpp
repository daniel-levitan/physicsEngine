#include "collision.h"
#include "../vector/vector2.h"
#include "supportPoint.h"

#include "../graphics/circle.h"
#include "../graphics/polygon.h"


/*********************
 * Circle vs Circle  *
 *********************/
float Collision::calculatePenetrationDepthOfCircles(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();

	Vector2 direction = centroidB - centroidA;

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();

	return radiusA + radiusB - direction.length();
}

bool Collision::checkCircleCircleBool(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();
	Vector2 direction = centroidB - centroidA;
	float directionLength = direction.length();

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();
	float sumRadius = radiusA + radiusB;

	if (directionLength < sumRadius) {
		return true;
	}

	return false;
}

std::unique_ptr<Manifold> Collision::checkCircleCircle(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();
	// Vector2 direction = centroidB - centroidA;
	Vector2 direction = Sub(centroidB, centroidA);

	float directionLength = direction.length();

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();
	float sumRadius = radiusA + radiusB;

	float depth = directionLength - sumRadius;

	if (directionLength < sumRadius) {
	// if (depth < 0) {
		// Normalizing with scale to get a new vector
		Vector2 penetrationNormal = Scale(direction, 1 / directionLength); 
		// float penetrationDepth = directionLength - sumRadius; 
		// Vector2 penetrationPoint = centroidA + Scale(penetrationNormal, radiusA);
		Vector2 penetrationPoint = Add(centroidA, Scale(penetrationNormal, radiusA));

		// return new Manifold(penetrationDepth * -1, penetrationNormal, penetrationPoint);
		return std::make_unique<Manifold>(depth * -1, penetrationNormal, penetrationPoint);
	}

	return nullptr;
}


/************************
 *  Polygon vs Polygon  *
 ************************/
// Checking for Polygon collisions method 1
/** If there is at least 1 vertice for which I cannot find a support point, the polygons
 *  don't collide. */
std::unique_ptr<SupportPoint> Collision::findSupportPoint(Vector2 normal, Vector2 pointOnEdge, std::vector<Vector2> otherPolygonVertices) {
   	// float maxPenetrationDepth = -std::numeric_limits<float>::infinity();
   	float maxPenetrationDepth = 0;
	std::unique_ptr<SupportPoint> supportPoint = nullptr;

	// int i = 0;
    for (const auto& vertex : otherPolygonVertices) {
        Vector2 verticeToEdge = Sub(vertex, pointOnEdge); // vertex - pointOnEdge;
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

/** Looks for the best(deepest) contact point in which the two polygons touched. **/
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
	
	std::unique_ptr<Manifold> contactPointPol2 = getContactPoint(pol2, pol1);	
	if (!contactPointPol2) 
		return nullptr;
	
	if (contactPointPol1->getDepth() < contactPointPol2->getDepth())
		return std::make_unique<Manifold>(contactPointPol1->getDepth(), contactPointPol1->getNormal(), contactPointPol1->getPenetrationPoint());
	else
		return std::make_unique<Manifold>(contactPointPol2->getDepth(), contactPointPol2->getNormal(), contactPointPol2->getPenetrationPoint());
}

// Checking for Polygon collisions method 2
bool Collision::checkPolygonPolygonSAT(Polygon& pol1, Polygon& pol2) {
	Polygon *poly1 = &pol1;
	Polygon *poly2 = &pol2;

	for (size_t shape = 0; shape < 2; shape++) {
		if (shape == 1) {
			poly1 = &pol2;
			poly2 = &pol1;
		}

		std::vector<Vector2> vertices1 = poly1->getVertices();
	    int size1 = vertices1.size();
		for (size_t i = 0; i < size1; i++) {
			
			// get the axis of projection
			Vector2 edge = vertices1[(i + 1) % size1] - vertices1[i];
			Vector2 axisProj = edge.getNormal();
			// axisProj.normalize();

			// calculating the min and max projection for shape 1
			float minPol1 = std::numeric_limits<float>::infinity();
			float maxPol1 = -std::numeric_limits<float>::infinity();

			for (const auto& v : vertices1) {
				float vertProj = v.dotProduct(axisProj);
				minPol1 = std::min(minPol1, vertProj);
				maxPol1 = std::max(maxPol1, vertProj);
			}

			// calculating the min and max projection for shape 2
			float minPol2 = std::numeric_limits<float>::infinity();
			float maxPol2 = -std::numeric_limits<float>::infinity();

			for (const auto& v : poly2->getVertices()) {
				float vertProj = v.dotProduct(axisProj);
				minPol2 = std::min(minPol2, vertProj);
				maxPol2 = std::max(maxPol2, vertProj);
			}

			/*
                     Pol 1                                 
				|------------|         Pol2
              minP1        maxP1   |-----------|                           There is no collision if:
								 minP2        maxP2                              maxP1 < minP2
                                                                                     OR
				                        OR                                       maxP2 < minP1
			                                                    Pol 1
				                       Pol2                |------------|
                                   |-----------|          minP1        maxP1
								 minP2        maxP2
			
			*/
			/* If the shapes don't collide in at least 1 axis, there is no collision */
			// if (maxPol1 < minPol2 || minPol1 > maxPol2)
				// return false;

			if (!(maxPol2 >= minPol1 && maxPol1 >= minPol2))
				return false;
		}		
	}

	/** If the shape collides in all axis, there is a collision
	 *  The SAT look for at least one axis without collision, that's why we return false in this 
	 *  case, otherwise we return true.
	 */ 
	return true;
}

std::unique_ptr<Manifold> Collision::resPolygonPolygonSAT(Polygon& pol1, Polygon& pol2) {
	Polygon *poly1 = &pol1;
	Polygon *poly2 = &pol2;

	float overlap = std::numeric_limits<float>::infinity();
	float depth = std::numeric_limits<float>::max();
    Vector2 contactPoint(0, 0);
    Vector2 normal(0,0);
    bool contactPointFound = false;

	for (size_t shape = 0; shape < 2; shape++) {
		if (shape == 1) {
			poly1 = &pol2;
			poly2 = &pol1;
		}

		std::vector<Vector2> vertices1 = poly1->getVertices();
	    int size1 = vertices1.size();
	    for (size_t i = 0; i < size1; i++) {
			
			// get the axis of projection
			Vector2 edge = vertices1[(i + 1) % size1] - vertices1[i];
			Vector2 axisProj = edge.getNormal();
			axisProj.normalize();

			// calculating the min and max projection for shape 1
			float minPol1 = std::numeric_limits<float>::infinity();
			float maxPol1 = -std::numeric_limits<float>::infinity();

			for (const auto& v : vertices1) {
				float vertProj = v.dotProduct(axisProj);
				minPol1 = std::min(minPol1, vertProj);
				maxPol1 = std::max(maxPol1, vertProj);
			}

			// calculating the min and max projection for shape 2
			float minPol2 = std::numeric_limits<float>::infinity();
			float maxPol2 = -std::numeric_limits<float>::infinity();

			for (const auto& v : poly2->getVertices()) {
				float vertProj = v.dotProduct(axisProj);
				minPol2 = std::min(minPol2, vertProj);
				maxPol2 = std::max(maxPol2, vertProj);
			}

			// Calculate the overlap 
			overlap = std::min(std::min(maxPol1, maxPol2) - std::max(minPol1, minPol2), overlap);

			if (maxPol1 < minPol2 || minPol1 > maxPol2)
				return nullptr;

			float axisDepth = std::min(maxPol1 - minPol2, maxPol2 - minPol1);
			if (axisDepth < depth) {
				depth = axisDepth;
				normal = axisProj;

	            // float overlapStart = std::max(min1, min2);
	            // float overlapEnd = std::min(max1, max2);
	  
	  			// for (const auto& vertex : vertices1) {
	            //     float projection = vertex.dotProduct(axis);
	            //     if (projection >= overlapStart && projection <= overlapEnd) {
	            //         contactPoint = vertex;
	            //         contactPointFound = true;
	            //         break;
	            //     }
	            // }
			}

			// if (!(maxPol2 >= minPol1 && maxPol1 >= minPol2)) 
				// return 0;
		}		
	}

	// If we got here, the shapes overlap and we can resolve the collision
	// Vector2 direction = pol2.getCentroid() - pol1.getCentroid();
	// direction.normalize();
	// Vector2 push = Scale(direction, -1 * overlap);
	// // Vector2 push = Scale(direction, -.4999 * overlap);
	// pol1.move(push);

	depth /= normal.length();
	normal.normalize();

	Vector2 direction = pol2.getCentroid() - pol1.getCentroid();
	if (direction.dotProduct(normal) < 0.0f) 
		normal.scale(-1);
	
    if (!contactPointFound) {
        // Default to centroid-based guess if contact point is not found
        contactPoint = Scale((pol1.getCentroid() + pol2.getCentroid()), 0.5f);
    }

	// return overlap;
	// depth, normal, contactPoint
	return std::make_unique<Manifold>(depth, normal, contactPoint);
}

// Checking for Polygon collisions method #3
/**
 * In this algorithm we check every edge of one polygon against every diagonal of the other polygon.
 * If any diagonal crosses any edge, we have a collision.
 */
bool Collision::checkPolygonPolygonDIAG(Polygon& pol1, Polygon& pol2) {
	Polygon *poly1 = &pol1;
	Polygon *poly2 = &pol2;

	for (size_t shape = 0; shape < 2; shape++) {
		if (shape == 1) {
			poly1 = &pol2;
			poly2 = &pol1;
		}

		// Get diagonals of polygon1
		for (const auto& v1 : poly1->getVertices()) {

			// Vector2 diagonal = vertices2[i] - poly1.getCentroid();
			Vector2 line_r1s = poly1->getCentroid();
			Vector2 line_r1e = v1;


			std::vector<Vector2> vertices2 = poly2->getVertices();
	    	int size2 = vertices2.size();

    		// Get edges of polygon2
    		for (size_t j = 0; j < size2; j++) {
				// Vector2 edge = vertices2[(i + 1) % size2] - vertices2[i];	
				Vector2 line_r2s = vertices2[j];
				Vector2 line_r2e = vertices2[(j + 1) % size2];


				/*https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect*/
				// Check if they are crossing
				float h = (line_r2e.getX() - line_r2s.getX()) * (line_r1s.getY() - line_r1e.getY()) - (line_r1s.getX() - line_r1e.getX()) * (line_r2e.getY() - line_r2s.getY());
				float t1 = ((line_r2s.getY() - line_r2e.getY()) * (line_r1s.getX() - line_r2s.getX()) + (line_r2e.getX() - line_r2s.getX()) * (line_r1s.getY() - line_r2s.getY())) / h;
				float t2 = ((line_r1s.getY() - line_r1e.getY()) * (line_r1s.getX() - line_r2s.getX()) + (line_r1e.getX() - line_r1s.getX()) * (line_r1s.getY() - line_r2s.getY())) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f) 
					return true;
					
    		}
		}
        /*         *  *------*    In this algorithm we check every edge of one polygon 
		          / \ |      |    against every diagonal of the other polygon.
     	         /   \|  .   |
                /     \ /    |    If any diagonal crosses any edge, we have a collision.
               /      |\     |
              /       *-\----*
             /           \
     	    *-------------*          
		*/			
	}
	return false;
}

bool Collision::resPolygonPolygonDIAG(Polygon& pol1, Polygon& pol2) {
	Polygon *poly1 = &pol1;
	Polygon *poly2 = &pol2;

	for (size_t shape = 0; shape < 2; shape++) {
		if (shape == 1) {
			poly1 = &pol2;
			poly2 = &pol1;
		}

		// Get diagonals of polygon1
		for (const auto& v1 : poly1->getVertices()) {

			// Vector2 diagonal = vertices2[i] - poly1.getCentroid();
			Vector2 line_r1s = poly1->getCentroid();
			Vector2 line_r1e = v1;

			Vector2 displacement(0, 0);
			std::vector<Vector2> vertices2 = poly2->getVertices();
	    	int size2 = vertices2.size();

    		// Get edges of polygon2
    		for (size_t j = 0; j < size2; j++) {
				// Vector2 edge = vertices2[(i + 1) % size2] - vertices2[i];	
				Vector2 line_r2s = vertices2[j];
				Vector2 line_r2e = vertices2[(j + 1) % size2];


				/*https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect*/
				// Check if they are crossing
				float h = (line_r2e.getX() - line_r2s.getX()) * (line_r1s.getY() - line_r1e.getY()) - (line_r1s.getX() - line_r1e.getX()) * (line_r2e.getY() - line_r2s.getY());
				float t1 = ((line_r2s.getY() - line_r2e.getY()) * (line_r1s.getX() - line_r2s.getX()) + (line_r2e.getX() - line_r2s.getX()) * (line_r1s.getY() - line_r2s.getY())) / h;
				float t2 = ((line_r1s.getY() - line_r1e.getY()) * (line_r1s.getX() - line_r2s.getX()) + (line_r1e.getX() - line_r1s.getX()) * (line_r1s.getY() - line_r2s.getY())) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f) {
					// return true;
					float newX = displacement.getX();
					newX += (1.0f - t1) * (line_r1e.getX() - line_r1s.getX());
					displacement.setX(newX);
					// displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);

					float newY = displacement.getY();
					newY += (1.0f - t1) * (line_r1e.getY() - line_r1s.getY());
					displacement.setY(newY);
					// displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
				}
    		}
    		displacement.scale(shape == 0 ? -1 : 1);
    		poly1->move(displacement);
		}	
	}
	return false;
}

// Checking for polygon collisions method #4
void Collision::projectVertices(std::vector<Vector2>& vertices, Vector2& axis, float& min, float& max) {
	min = std::numeric_limits<float>::max();
	max = std::numeric_limits<float>::lowest();

	for (const auto& v : vertices) {
		float proj = v.dotProduct(axis);

		if (proj < min) { min = proj; }
		if (proj > max) { max = proj; }
	}
}

/**
 * The algorithm look for the minimum and maximum values of projection for each polygon
 * in all projection axis. If any gap between the two polygons is found in a projection,
 * the polygons can be separated and then are not colliding.
 * 
 * There are two loops for each pair. We first project all vertices from the two polygons
 * in each projection axis of the first polygon. Then we do the same for the second polygon.
 * */
bool Collision::intersectPolygons(Polygon& pol1, Polygon& pol2) {
	float min1, max1;
	float min2, max2;

	std::vector<Vector2> vertices2 = pol2.getVertices();
	std::vector<Vector2> vertices1 = pol1.getVertices();
    int size1 = vertices1.size();
    for (size_t i = 0; i < size1; i++) {
		Vector2 va = vertices1[i];
		Vector2 vb = vertices1[(i + 1) % size1];
		Vector2 edge = vb - va;
		Vector2 axis = edge.getNormal();

		Collision::projectVertices(vertices1, axis, min1, max1);
		Collision::projectVertices(vertices2, axis, min2, max2);

		if (max1 < min2 || min1 > max2)
			return false;
	}

	int size2 = vertices2.size();
    for (size_t i = 0; i < size2; i++) {
		Vector2 va = vertices2[i];
		Vector2 vb = vertices2[(i + 1) % size2];
		Vector2 edge = vb - va;
		Vector2 axis = edge.getNormal();

		Collision::projectVertices(vertices1, axis, min1, max1);
		Collision::projectVertices(vertices2, axis, min2, max2);

		if (max1 < min2 || min1 > max2)
			return false;
	}

	return true;
}

std::unique_ptr<Manifold> Collision::resIntersectPolygons(Polygon& pol1, Polygon& pol2) {
	Vector2 normal(0,0);
	float depth = std::numeric_limits<float>::max();
    Vector2 contactPoint(0, 0);
    bool contactPointFound = false;


	float min1, max1;
	float min2, max2;

	std::vector<Vector2> vertices2 = pol2.getVertices();
	std::vector<Vector2> vertices1 = pol1.getVertices();
    int size1 = vertices1.size();
    for (size_t i = 0; i < size1; i++) {
		Vector2 va = vertices1[i];
		Vector2 vb = vertices1[(i + 1) % size1];
		Vector2 edge = vb - va;
		Vector2 axis = edge.getNormal();

		Collision::projectVertices(vertices1, axis, min1, max1);
		Collision::projectVertices(vertices2, axis, min2, max2);

		if (max1 < min2 || min1 > max2)
			return nullptr;

		float axisDepth = std::min(max1 - min2, max2 - min1);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;

            // float overlapStart = std::max(min1, min2);
            // float overlapEnd = std::min(max1, max2);
  
  			// for (const auto& vertex : vertices1) {
            //     float projection = vertex.dotProduct(axis);
            //     if (projection >= overlapStart && projection <= overlapEnd) {
            //         contactPoint = vertex;
            //         contactPointFound = true;
            //         break;
            //     }
            // }
		}
	}

	int size2 = vertices2.size();
    for (size_t i = 0; i < size2; i++) {
		Vector2 va = vertices2[i];
		Vector2 vb = vertices2[(i + 1) % size2];
		Vector2 edge = vb - va;
		Vector2 axis = edge.getNormal();

		Collision::projectVertices(vertices1, axis, min1, max1);
		Collision::projectVertices(vertices2, axis, min2, max2);

		if (max1 < min2 || min1 > max2)
			return nullptr;
		float axisDepth = std::min(max1 - min2, max2 - min1);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;

            // float overlapStart = std::max(min1, min2);
            // float overlapEnd = std::min(max1, max2);

            // for (const auto& vertex : vertices2) {
            //     float projection = vertex.dotProduct(axis);
            //     if (projection >= overlapStart && projection <= overlapEnd) {
            //         contactPoint = vertex;
            //         contactPointFound = true;
            //         break;
            //     }
            // }
		}
	}

	depth /= normal.length();
	normal.normalize();

	Vector2 direction = pol2.getCentroid() - pol1.getCentroid();
	if (direction.dotProduct(normal) < 0.0f) 
		normal.scale(-1);
	
    if (!contactPointFound) {
        // Default to centroid-based guess if contact point is not found
        contactPoint = Scale((pol1.getCentroid() + pol2.getCentroid()), 0.5f);
    }

	// depth, normal, contactPoint
	return std::make_unique<Manifold>(depth, normal, contactPoint);
}


/**********************
 * Circle vs Polygon  *
 **********************/
// Checking for collision between circle and polygon
bool Collision::betweenEdges(Circle& circ, Polygon& pol) {
	std::vector<Vector2> vertices = pol.getVertices();
	int size = vertices.size();	

	for (size_t i = 0; i < size; i++) {
		Vector2 edge = vertices[(i + 1) % size] - vertices[i];
		Vector2 vertToCircle = circ.getCentroid() - vertices[i];
		float projToEdge = vertToCircle.dotProduct(Normalize(edge));

		// Here I should actually check vertices against circle
		if (projToEdge > 0 and projToEdge < edge.length()) 
			return true;		
 	}

 	return false;
}

float pointToLineSegmentDistance(const Vector2& P, const Vector2& A, const Vector2& B) {
    Vector2 AB = B - A;
    Vector2 AP = P - A;

    // Projection factor t
    float t = AP.dotProduct(AB) / AB.dotProduct(AB);

    // Clamp t to [0, 1]
    t = std::max(0.0f, std::min(1.0f, t));

    // Closest point on the segment
    Vector2 Q = A + Scale(AB, t);

    // Distance from P to Q
    Vector2 PQ = P - Q;
    return PQ.length();
}

std::unique_ptr<Manifold> Collision::checkCirclePolygonEdges(Circle& circ, Polygon& pol) {
	std::vector<Vector2> normals = pol.getNormals();
	std::vector<Vector2> vertices = pol.getVertices();
	int size = vertices.size();	
	Vector2 center = circ.getCentroid();
	float radius = circ.getRadius();

	for (size_t i = 0; i < size; i++) {
		Vector2 edge = vertices[(i + 1) % size] - vertices[i];
		Vector2 vertToCircle = circ.getCentroid() - vertices[i];
		float projToEdge = vertToCircle.dotProduct(Normalize(edge));

		Vector2 direction = center - vertices[i];
		float projToNormal = direction.dotProduct(normals[i]);

		// Here I should actually check vertices against circle
		if (projToEdge > 0 and projToEdge < edge.length() and projToNormal > 0) {
			float depth = projToNormal - radius;
			if (depth < 0.0f)
				return std::make_unique<Manifold>(depth * -1, Scale(normals[i], -1), center + Scale(normals[i], radius * -1));		
		}	
 	}

 	return nullptr;
}

bool Collision::checkCirclePolygonEdges_v2(Circle& circ, Polygon& pol) {
	std::vector<Vector2> normals = pol.getNormals();
	std::vector<Vector2> vertices = pol.getVertices();
	int size = vertices.size();	
	Vector2 center = circ.getCentroid();
	float radius = circ.getRadius();

	for (size_t i = 0; i < size; i++) {
		Vector2 edge = vertices[(i + 1) % size] - vertices[i];
		Vector2 vertToCircle = circ.getCentroid() - vertices[i];
		float projToEdge = vertToCircle.dotProduct(Normalize(edge));

		Vector2 direction = center - vertices[i];
		float projToNormal = direction.dotProduct(normals[i]);

		// Here I should actually check vertices against circle
		if (projToEdge > 0 and projToEdge < edge.length() and projToNormal > 0) {
			float depth = projToNormal - radius;
			if (depth < 0.0f)
				return true;
		}		
 	}

 	return false;
}

bool Collision::checkCirclePolygonEdgesBool(Circle& circ, Polygon& pol) {
	std::vector<Vector2> normals = pol.getNormals();
	std::vector<Vector2> vertices = pol.getVertices();
	int size = vertices.size();	
	Vector2 center = circ.getCentroid();
	float radius = circ.getRadius();

	int iClosest = 0;
	float closestDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < size; i++) {
		float distance = pointToLineSegmentDistance(center, vertices[(i + 1) % size], vertices[i]);
		// get the edge closer to the center of the circle
		if (distance < closestDistance) {
			closestDistance = distance;
			iClosest = i;
		}
 	}
	
	Vector2 direction = center - vertices[iClosest];
	float projToNormal = direction.dotProduct(normals[iClosest]);
	float depth = projToNormal - radius;
	if (depth < 0.0f)
		return true;
	
	return false;
}

bool Collision::checkCirclePolygonCornersBool(Circle& circ, Polygon& pol) {
	Vector2 center = circ.getCentroid();

	for (const auto& v : pol.getVertices()) {         // For every vertice
		Vector2 direction = center - v;               
		float directionLength = direction.length();   // Get the len from the vertice to the circle

		if (directionLength < circ.getRadius())       // Check if they are on top of each other
			return true;
	}

	return false;
}

/*
	static circleVsPolygonCorners(shapeCircle, shapePolygon){
		
		let verticesLength = shapePolygon.vertices.length;
		
		for(let i=0; i<verticesLength;i++){
		
			let currVertex = shapePolygon.vertices[i];
			let dirToCentroidCircle = Sub(currVertex, shapeCircle.centroid);
		
			if(dirToCentroidCircle.Length2() < shapeCircle.radius*shapeCircle.radius){
				let penetration = shapeCircle.radius - dirToCentroidCircle.Length();
				dirToCentroidCircle.Normalize();
		
				return new CollisionManifold(penetration, Scale(dirToCentroidCircle,1),currVertex );
			}
		}
		return null;
	}
*/
std::unique_ptr<Manifold> Collision::checkCirclePolygonCorners(Circle& circ, Polygon& pol) {
	float penetration;

	Vector2 center = circ.getCentroid();
	for (const auto& v : pol.getVertices()) {         // For every vertice
		Vector2 direction = Sub(v, center);               
		// float directionLength = direction.length();   // Get the len from the vertice to the circle

		float radius = circ.getRadius();
		if (direction.length2() < radius * radius) {    // Check if they are on top of each other
			penetration = radius - direction.length();
			direction.normalize();
			return std::make_unique<Manifold>(penetration * -1, direction, v);
		}
	}

	return nullptr;
}

/*
std::unique_ptr<Manifold> Collision::checkCirclePolygonCorners(Circle& circ, Polygon& pol) {
	Vector2 center = circ.getCentroid();

	for (const auto& v : pol.getVertices()) {         // For every vertice
		Vector2 direction = center - v;               
		float directionLength = direction.length();   // Get the len from the vertice to the circle

		if (directionLength < circ.getRadius())       // Check if they are on top of each other
			return std::make_unique<Manifold>((directionLength - circ.getRadius()) * -1, Normalize(direction), v);
	}

	return nullptr;
}
*/

std::unique_ptr<Manifold> Collision::checkCirclePolygon(Circle& circ, Polygon& pol) {
// bool Collision::checkCirclePolygon(Circle& circ, Polygon& pol) {
	// return checkCirclePolygonEdges_v2(circ, pol);
	// return betweenEdges(circ, pol);
	// if (betweenEdges(circ, pol)) 					
		// return checkCirclePolygonEdges_v2(circ, pol);
		// return checkCirclePolygonEdges(circ, pol);	
	auto result = checkCirclePolygonEdges(circ, pol);
	if (result)
		return result;
	return checkCirclePolygonCorners(circ, pol);	
}


/**************************************
 * General collision checking methods *
 **************************************/ 
// General check collision method
std::unique_ptr<Manifold> Collision::checkCollision(Shape& s1, Shape& s2) {
// bool Collision::checkCollision(Shape& s1, Shape& s2) {
    return s1.acceptCollision(s2);
}

// bool Collision::collisionDetection(Shape& s1, Shape& s2) {
// std::unique_ptr<Manifold> Collision::collisionDetection(Shape& s1, Shape& s2) {
std::unique_ptr<Manifold> Collision::collisionDetection(RigidBody& rb1, RigidBody& rb2) {
	Shape* s1 = rb1.getShape();
	Shape* s2 = rb2.getShape();
	return s1->acceptCollision(*s2);
}

void Collision::resolveCollision(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {

	// Calculating the linear impulse
	// For some reason I had to invert this here and the impulse direction
	Vector2 direction = Sub(rb1.getShape()->getCentroid(), rb2.getShape()->getCentroid());
	if (direction.dotProduct(manifold.getNormal()) > 0)
		manifold.setNormal(Scale(manifold.getNormal(), -1));

	Vector2 relativeVelocity = Sub(rb2.getVelocity(), rb1.getVelocity());
	double relativeVelocityProjected = relativeVelocity.dotProduct(manifold.getNormal());

	if (relativeVelocityProjected > 0)
		return;

	// This controls if the body is able to move or not
	if (rb1.isKinematic() && rb2.isKinematic())
		return;

	float invertedMassSum = rb1.getInvertedMass() + rb2.getInvertedMass();

	// Restitution Coeficiency - There are two ways to calculate it
	float e = 1; 
	// float e = std::min(rb1.getMaterial()->getBounce(), rb2.getMaterial()->getBounce());
	// float bounceSum = rb1.getMaterial()->getBounce() + rb2.getMaterial()->getBounce();
	// float e = (2 * rb1.getMaterial()->getBounce() * rb2.getMaterial()->getBounce()) / bounceSum;
	
	float j = -1 * (1 + e) * relativeVelocityProjected;
	j /= invertedMassSum;

	Vector2 impulse = Scale(manifold.getNormal(), j);
	Vector2 rb1Impulse = Scale(impulse, -1 * rb1.getInvertedMass());
	Vector2 rb2Impulse = Scale(impulse, 1 * rb2.getInvertedMass());

	rb1.setVelocity(Add(rb1.getVelocity(), rb1Impulse));
	rb2.setVelocity(Add(rb2.getVelocity(), rb2Impulse));
}

void Collision::positionCorrection(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
	float correctionPercentage = .2f;
	float amount = manifold.getDepth() / (rb1.getInvertedMass() + rb2.getInvertedMass() * correctionPercentage);
	Vector2 correctionVector = Scale(manifold.getNormal(), amount); 

	Vector2 rb1Movement = Scale(correctionVector, rb1.getInvertedMass() * +1);
	Vector2 rb2Movement = Scale(correctionVector, rb2.getInvertedMass() * -1);


	float correctionScale = 0.4;
	if (!rb1.isKinematic())
		rb1.getShape()->move(Scale(rb1Movement, correctionScale));

	if (!rb2.isKinematic())
		rb2.getShape()->move(Scale(rb2Movement, correctionScale));
}

