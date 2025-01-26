#include "rigid_body.h"

void RigidBody::addForce(Vector2 force) {
	forceAccumulator.add(force);
}

void RigidBody::addVelocity(Vector2 velocity) {
	velocityAccumulator.add(velocity);
}

void RigidBody::setVelocity(Vector2 velocity) {
	velocityAccumulator = velocity.copy();
}

Vector2 RigidBody::integrate(float delta_time) {
	return Vector2::Null;
}

void RigidBody::update(float delta_time) {
	integrate(delta_time);
}

void RigidBody::draw(SDL_Renderer* renderer) {
	shape->draw(renderer);
}

void RigidBody::move(Vector2 delta) {
	shape->move(delta);
}

void RigidBody::rotate(float radiansDelta) {
	shape->rotate(radiansDelta);
}