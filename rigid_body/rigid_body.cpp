#include "rigid_body.h"
#include "../utils/utils.h"
#include "../engine/src/constants.h"

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

void RigidBody::setImpulse(Vector2 newImpulse) {
    impulse = newImpulse.copy();
}

/** This function uses one of the integration methods right below it to calculate
 * 
 */
Vector2 RigidBody::integrate(float delta_time) {
    Vector2 deltaPosition = Vector2::Null;  
    
    deltaPosition = semiImplicitEuler(delta_time);
    // deltaPosition = forwardEuler(delta_time);
    // deltaPosition = midPointMethod(delta_time);
    // deltaPosition = rungeKutta4(delta_time);

    return deltaPosition;
}


/** The set of equations below is called the integration method or semi implicit Euler method.
  * It relies on basic physics:
  *    1. F = m * a
  *    2. V = V_0 +  a * t
  *    3. S = S_0 + v_0 * t + (a * t^2) / 2
  */
Vector2 RigidBody::semiImplicitEuler(float delta_time) {
    Vector2 acceleration = Scale(forceAccumulator, invertedMass); // F = m * a -> a = F * 1 / m
    velocityAccumulator.add(Scale(acceleration, delta_time));     // v_1 = v_0 + a * t
    Vector2 deltaPosition = Scale(velocityAccumulator, delta_time); // s = s_0 + v_0 * t + (a * t^2) / 2
    forceAccumulator = Vector2::Null;
    
    return deltaPosition;
}

/**
 * The order of the velocity update is changed.
 * */
Vector2 RigidBody::forwardEuler(float delta_time) {
    Vector2 acceleration = Scale(forceAccumulator, invertedMass); 
    Vector2 deltaPosition = Scale(velocityAccumulator, delta_time);
    
    velocityAccumulator.add(Scale(acceleration, delta_time));    
    
    return deltaPosition;
}

/** This method is a cross between the previous two.
 * */
Vector2 RigidBody::midPointMethod(float delta_time) {
    Vector2 acceleration = Scale(forceAccumulator, invertedMass); 
    Vector2 halfAcceleration = Scale(acceleration, 0.5f);
    velocityAccumulator.add(Scale(halfAcceleration, delta_time));     
    Vector2 deltaPosition = Scale(velocityAccumulator, delta_time);

    velocityAccumulator.add(Scale(halfAcceleration, delta_time));     

    return deltaPosition;
}

Vector2 computeAcceleration(Vector2 force, float invertedMass) {
    return Scale(force, invertedMass);
}

Vector2 RigidBody::rungeKutta4(float delta_time) {

    // k1
    Vector2 acceleration = computeAcceleration(forceAccumulator, invertedMass);
    Vector2 k1 = Scale(acceleration, delta_time);

    // k2
    Vector2 temporaryForce = Add(forceAccumulator, Scale(k1, 0.5));
    acceleration = computeAcceleration(forceAccumulator, invertedMass);
    Vector2 k2 = Scale(acceleration, delta_time);

    // k3
    temporaryForce = Add(forceAccumulator, Scale(k2, 0.5));
    acceleration = computeAcceleration(forceAccumulator, invertedMass);
    Vector2 k3 = Scale(acceleration, delta_time);

    // k4
    temporaryForce = Add(forceAccumulator, Scale(k3, 0.5));
    acceleration = computeAcceleration(forceAccumulator, invertedMass);
    Vector2 k4 = Scale(acceleration, delta_time);

    // Calculating velocity
    // (((k2 x 2) + k1) + ((k3 x 2) + k4)) / 6
    // (k1 + 2K2 + 2K3 + k4) / 6
    Vector2 deltaVelocity = Scale( Add( Add( k1, Scale(k2, 2) ), Add( Scale(k3, 2), k4) ), 1 / 6.0f );
    velocityAccumulator = velocityAccumulator + deltaVelocity;

    Vector2 deltaPosition = Scale(velocityAccumulator, delta_time);

    return deltaPosition;
}

void RigidBody::update(float delta_time) {
    Vector2 deltaPos = integrate(delta_time);
    shape->move(deltaPos);

    forceAccumulator = Vector2::Null;
    velocityAccumulator = Scale(velocityAccumulator, DAMPING);
    angularVelocity *= DAMPING;

    float deltaRotation = angularVelocity * delta_time;
    shape->rotate(deltaRotation);
}

void RigidBody::draw(SDL_Renderer* renderer) {
    shape->draw(renderer);
}

std::string RigidBody::toStringFandV() {
    std::string forceStr = "Force (" + Utils::to_string_with_precision(forceAccumulator.getX(),1) + ", " +
        Utils::to_string_with_precision(forceAccumulator.getY(),1) + ")";
    std::string velocityStr = "Velocity (" + Utils::to_string_with_precision(velocityAccumulator.getX(),1) + ", " +
        Utils::to_string_with_precision(velocityAccumulator.getY(),1) + ")";

    std::string result = forceStr + "\n";
    result += velocityStr;

    return result;
}

void RigidBody::move(Vector2 delta) {
    shape->move(delta);
}

void RigidBody::rotate(float radiansDelta) {
    shape->rotate(radiansDelta);
}

void RigidBody::calculateInertia(Shape& s) {
    inertia = s.calculateInertia(mass);
}
// std::unique_ptr<Manifold> Collision::checkCollision(Shape& s1, Shape& s2) {
// // bool Collision::checkCollision(Shape& s1, Shape& s2) {
//     return s1.acceptCollision(s2);
// }