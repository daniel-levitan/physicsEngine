#include "collision.h"
#include "../vector/vector2.h"
#include "supportPoint.h"

#include "../graphics/circle.h"
#include "../graphics/polygon.h"


/*********************
 * Circle vs Circle  *
 *********************/
std::unique_ptr<Manifold> Collision::checkCircleCircle(const Circle& c1, const Circle& c2) {
	Vector2 centroidA = c1.getCentroid();
	Vector2 centroidB = c2.getCentroid();

	Vector2 direction = Sub(centroidB, centroidA);
	float directionLength = direction.length();

	float radiusA = c1.getRadius();
	float radiusB = c2.getRadius();
	float sumRadius = radiusA + radiusB;

	float depth = directionLength - sumRadius;

	if (directionLength < sumRadius) {
		// Normalizing with scale to get a new vector
		Vector2 penetrationNormal = Scale(direction, 1 / directionLength); 
		Vector2 penetrationPoint = Add(centroidA, Scale(penetrationNormal, radiusA));

		return std::make_unique<Manifold>(depth * -1, Scale(penetrationNormal, -1), penetrationPoint);
	}

	return nullptr;
}


/************************
 *  Polygon vs Polygon  *
 ************************/
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
			}	
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

	return std::make_unique<Manifold>(depth, Scale(normal, -1), contactPoint);
}


/**********************
 * Circle vs Polygon  *
 **********************/
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
		if (projToEdge >= 0 and projToEdge <= edge.length() and projToNormal > 0) {
			float depth = projToNormal - radius;
			if (depth < 0.0f)
				return std::make_unique<Manifold>(depth * -1, Scale(normals[i], -1), center + Scale(normals[i], radius * -1));				
		}
 	}

 	return nullptr;
}

std::unique_ptr<Manifold> Collision::checkCirclePolygonCorners(Circle& circ, Polygon& pol) {
	float depth;
	std::vector<Vector2> vertices = pol.getVertices();
	int size = vertices.size();	

	for (size_t i = 0; i < size; i++) {
		Vector2 dirToCircle = circ.getCentroid() - vertices[i];

		float radius = circ.getRadius();
		if (dirToCircle.length2() < radius * radius) {
			depth = dirToCircle.length() - radius;
			dirToCircle.normalize();
			
			return std::make_unique<Manifold>(depth * -1, Scale(dirToCircle, -1), circ.getCentroid() - Scale(dirToCircle, radius));
		}
	}

	return nullptr;
}

std::unique_ptr<Manifold> Collision::checkCirclePolygon(Circle& circ, Polygon& pol) {
	auto result = checkCirclePolygonEdges(circ, pol);
	if (result)
		return result;
	return checkCirclePolygonCorners(circ, pol);
}


/**************************************
 * General collision checking methods *
 **************************************/ 
std::unique_ptr<Manifold> Collision::collisionDetection(RigidBody& rb1, RigidBody& rb2) {
	Shape* s1 = rb1.getShape();
	Shape* s2 = rb2.getShape();
	return s1->acceptCollision(*s2);
}

// void Collision::resolveLinearCollision_v1(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
void Collision::resolveLinearCollision(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
	// If the bodies are static, no need to check them
	if (manifold.getNormal().dotProduct(rb2.getShape()->getCentroid() - rb1.getShape()->getCentroid()) < 0) {
		manifold.setNormal(Scale(manifold.getNormal(), -1));
	}
	if (rb1.isStatic() && rb2.isStatic())
		return;
	
	Vector2 relativeVelocity = Sub(rb2.getLinearVelocity(), rb1.getLinearVelocity());
	double relativeVelocityProjected = relativeVelocity.dotProduct(manifold.getNormal());

	// If the bodies are already moving apart
	if (relativeVelocityProjected > 0)
		return;

	// Based on https://www.chrishecker.com/images/e/e7/Gdmphys3.pdf
	float e = std::min(rb1.getMaterial()->getRestitution(), rb2.getMaterial()->getRestitution());
	float j = -(1 + e) * relativeVelocityProjected;
	j /= (rb1.getInvertMass() + rb2.getInvertMass());

	Vector2 impulse = Scale(manifold.getNormal(), j);
	rb1.addLinearVelocity(Scale(impulse, -1 * rb1.getInvertMass()));
	rb2.addLinearVelocity(Scale(impulse, +1 * rb2.getInvertMass()));
}


void Collision::resolveLinearCollisionOriginal(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
	Vector2 direction = Sub(rb1.getShape()->getCentroid(), rb2.getShape()->getCentroid());
	if (direction.dotProduct(manifold.getNormal()) > 0)
		manifold.setNormal(Scale(manifold.getNormal(), -1));

	Vector2 relativeVelocity = Sub(rb2.getLinearVelocity(), rb1.getLinearVelocity());
	double relativeVelocityProjected = relativeVelocity.dotProduct(manifold.getNormal());

	if (relativeVelocityProjected > 0)
		return;

	// This controls if the body is able to move or not
	if (rb1.isStatic() && rb2.isStatic())
		return;

	float invertedMassSum = rb1.getInvertMass() + rb2.getInvertMass();

	// Restitution Coeficiency - There are two ways to calculate it
	float e = std::min(rb1.getMaterial()->getRestitution(), rb2.getMaterial()->getRestitution());
	// float bounceSum = rb1.getMaterial()->getBounce() + rb2.getMaterial()->getBounce();
	// float e = (2 * rb1.getMaterial()->getBounce() * rb2.getMaterial()->getBounce()) / bounceSum;

	float j = -1 * (1 + e) * relativeVelocityProjected;
	j /= invertedMassSum;

	Vector2 impulse = Scale(manifold.getNormal(), j);
	Vector2 rb1Impulse = Scale(impulse, -1 * rb1.getInvertMass());
	Vector2 rb2Impulse = Scale(impulse, +1 * rb2.getInvertMass());

	rb1.setLinearVelocity(Add(rb1.getLinearVelocity(), rb1Impulse));
	rb2.setLinearVelocity(Add(rb2.getLinearVelocity(), rb2Impulse));
}

void Collision::resolveCollision(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
	// This controls if the body is able to move or not
	if (rb1.isStatic() && rb2.isStatic())
		return;
	
	// Calculating the linear impulse
	Vector2 penetrationToCentroidRb1 = Sub(manifold.getPenetrationPoint(), rb1.getShape()->getCentroid());
	Vector2 penetrationToCentroidRb2 = Sub(manifold.getPenetrationPoint(), rb2.getShape()->getCentroid());

	Vector2 angularVelocityPenetretationCentroidRb1 = 
			Vector2(-1 * rb1.getAngularVelocity() * penetrationToCentroidRb1.getY(),
					rb1.getAngularVelocity() * penetrationToCentroidRb1.getX());
	Vector2 angularVelocityPenetretationCentroidRb2 = 
			Vector2(-1 * rb2.getAngularVelocity() * penetrationToCentroidRb2.getY(),
					rb2.getAngularVelocity() * penetrationToCentroidRb2.getX());

	Vector2 relativeVelocityRb1 = Add(rb1.getLinearVelocity(), angularVelocityPenetretationCentroidRb1);
	Vector2 relativeVelocityRb2 = Add(rb2.getLinearVelocity(), angularVelocityPenetretationCentroidRb2);

	Vector2 relativeVelocity = Sub(relativeVelocityRb2, relativeVelocityRb1);

	float velocityOnNormal = relativeVelocity.dotProduct(manifold.getNormal());

	if (velocityOnNormal > 0)
		return;


	// Restitution Coeficiency - There are two ways to calculate it
	// float e = 1; 
	float e = std::min(rb1.getMaterial()->getRestitution(), rb2.getMaterial()->getRestitution());
	float penetrationToCentroidCrossNormalRb1 = penetrationToCentroidRb1.cross(manifold.getNormal());
	float penetrationToCentroidCrossNormalRb2 = penetrationToCentroidRb2.cross(manifold.getNormal());

	float invertMassSum = rb1.getInvertMass() + rb2.getInvertMass();

	float rb1InvertedInertia = rb1.getInvertedInertia();
	float rb2InvertedInertia = rb2.getInvertedInertia();
	float crossNSum = penetrationToCentroidCrossNormalRb1 * penetrationToCentroidCrossNormalRb1 * rb1InvertedInertia +
					  penetrationToCentroidCrossNormalRb2 * penetrationToCentroidCrossNormalRb2 * rb2InvertedInertia;

	float j = -(1 + e) * velocityOnNormal;
	j /= (invertMassSum + crossNSum);

	Vector2 impulse = Scale(manifold.getNormal(), j);
	
	Vector2 rb1Impulse = Scale(impulse, rb1.getInvertMass());
	Vector2 rb2Impulse = Scale(impulse, rb2.getInvertMass());

	rb1.setLinearVelocity(Sub(rb1.getLinearVelocity(), rb1Impulse));
	rb2.setLinearVelocity(Add(rb2.getLinearVelocity(), rb2Impulse));

	rb1.setAngularVelocity(rb1.getAngularVelocity() * -penetrationToCentroidCrossNormalRb1 * j * rb1.getInvertedInertia());
	rb2.setAngularVelocity(rb2.getAngularVelocity() * penetrationToCentroidCrossNormalRb2 * j * rb2.getInvertedInertia());
}

void Collision::positionCorrection(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
	float correctionPercentage = .2f;
	float amount = manifold.getDepth() / (rb1.getInvertMass() + rb2.getInvertMass() * correctionPercentage);
	Vector2 correctionVector = Scale(manifold.getNormal(), amount); 

	Vector2 rb1Movement = Scale(correctionVector, rb1.getInvertMass() * -1);
	Vector2 rb2Movement = Scale(correctionVector, rb2.getInvertMass() * +1);

	if (!rb1.isStatic())
		rb1.getShape()->move(rb1Movement);

	if (!rb2.isStatic())
		rb2.getShape()->move(rb2Movement);

	// float correctionScale = 0.6;
	// if (!rb1.isStatic())
		// rb1.getShape()->move(Scale(rb1Movement, correctionScale));

	// if (!rb2.isStatic())
		// rb2.getShape()->move(Scale(rb2Movement, correctionScale));
}


void Collision::simplePositionCorrection(RigidBody& rb1, RigidBody& rb2, Manifold& manifold) {
	Vector2 push;

    push = Scale(manifold.getNormal(), manifold.getDepth() * 0.5);
    rb1.getShape()->move(push);

    push = Scale(manifold.getNormal(), manifold.getDepth() * -0.5);
    rb2.getShape()->move(push);
}
	

