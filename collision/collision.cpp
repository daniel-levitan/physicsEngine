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

/** If there is at least 1 vertice for which I cannot find a support point, the polygons
 *  don't collide. */
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

// float Collision::resPolygonPolygonSAT(Polygon& pol1, Polygon& pol2) {
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

bool Collision::resPolygonPolygonDIAG(Polygon& pol1, Polygon& pol2) {
	Polygon *poly1 = &pol1;
	Polygon *poly2 = &pol2;

	Vector2 displacement = Vector2(0, 0);

	for (size_t shape = 0; shape < 2; shape++) {
		if (shape == 1) {
			poly1 = &pol2;
			poly2 = &pol1;
		}

		// Get diagonals of polygon1
		for (const auto& v : poly1->getVertices()) {

			// Vector2 diagonal = vertices2[i] - poly1.getCentroid();
			Vector2 line_r1s = poly1->getCentroid();
			Vector2 line_r1e = v;

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

				// Now we accumulate the displacement
				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f) {
					float newX = displacement.getX();
					float newY = displacement.getY();
					newX = newX + (1.0f - t1) * (line_r1e.getX() - line_r1s.getX());
					displacement.setX(newX);
					newY = newY + (1.0f - t1) * (line_r1e.getY() - line_r1s.getY());
					displacement.setY(newY);

                    Vector2 push = Scale(displacement, (shape == 0 ? -1 : 1));

                    // If I only want the polygon to refrain from entering the other polygons
                    pol1.move(push);

                    // If I want the two parts to move
					// pol1.move(Scale(push, 0.5));
					// pol2.move(Scale(push, -0.5));
					return true;
				}				
    		}
		}				
	}
	return false;
}

void Collision::projectVertices(std::vector<Vector2>& vertices, Vector2& axis, float& min, float& max) {
	min = std::numeric_limits<float>::max();
	max = std::numeric_limits<float>::lowest();

	for (const auto& v : vertices) {
		float proj = v.dotProduct(axis);

		if (proj < min) { min = proj; }
		if (proj > max) { max = proj; }
	}
}

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