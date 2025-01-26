#include "rigid_body.h"
#include "../utils/utils.h"

void RigidBody::addForce(Vector2 force) {
	forceAccumulator.add(force);
}

void RigidBody::setForce(Vector2 force) {
	forceAccumulator = force.copy();

}

void RigidBody::addVelocity(Vector2 velocity) {
	velocityAccumulator.add(velocity);
}

void RigidBody::setVelocity(Vector2 velocity) {
	velocityAccumulator = velocity.copy();
}

Vector2 RigidBody::integrate(float delta_time) {
	
	Vector2 acceleration = Scale(forceAccumulator, invertedMass); // F = m * a -> a = F * 1 / m
	velocityAccumulator.add(Scale(acceleration, delta_time));     // v_1 = v_0 + a * t
	Vector2 deltaPosition = Scale(velocityAccumulator, delta_time); // s = s_0 + v_0 * t + (a * t^2) / 2

	forceAccumulator = Vector2::Null;
	return deltaPosition;
}

void RigidBody::update(float delta_time) {
	Vector2 deltaPos = integrate(delta_time);
	shape->move(deltaPos);
}

void RigidBody::draw(SDL_Renderer* renderer) {
	shape->draw(renderer);
}

std::string RigidBody::toStringFandV() {
	std::string forceStr = "Force (" + Utils::to_string_with_precision(forceAccumulator.getX(),1) + ", " +
        Utils::to_string_with_precision(forceAccumulator.getY(),1) + ")";
    return forceStr;
}

void RigidBody::move(Vector2 delta) {
	shape->move(delta);
}

void RigidBody::rotate(float radiansDelta) {
	shape->rotate(radiansDelta);
}