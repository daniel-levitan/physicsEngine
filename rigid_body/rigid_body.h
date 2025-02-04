#ifndef _rigid_body_h
#define _rigid_body_h

#include "../graphics/shape.h"
#include "../vector/vector2.h"
#include "../material/material.h"


class RigidBody {
protected:
    std::unique_ptr<Shape> shape;
    float mass;
    float invertMass;
    float inertia;
    float invertedInertia;

    Vector2 forceAccumulator; 
    Vector2 velocityAccumulator;
    float angularVelocity;

    Material material;
    bool isStaticState = false;

    Vector2 impulse;

public:
    RigidBody(std::unique_ptr<Shape> shape, float mass = 1.0f, float restitution = 1.0f, float friction = 0.0f);

    // Force and velocity methods
    void addForce(Vector2 force);
    void setForce(Vector2 force);
    
    void addLinearVelocity(Vector2 velocity);
    void setLinearVelocity(Vector2 velocity);
    Vector2 getLinearVelocity() { return velocityAccumulator; };

    float getMass() { return mass; };
    float getInvertMass() { return invertMass; };
    float getInvertedInertia() { return invertedInertia; };

    void addAngularVelocity(float velocity);
    float getAngularVelocity() { return angularVelocity; };
    void setAngularVelocity(float angularVelocity);

    void setImpulse(Vector2 impulse);
    Vector2 getImpulse() { return impulse;};

    void draw(SDL_Renderer* renderer);
    Shape* getShape() { return shape.get(); }     // std::unique_ptr<Shape> getShape();
    Material* getMaterial() { return &material; };

    // Methods for updating the position
    void update(float delta_time);
    Vector2 integrate(float delta_time);
    bool isStatic() { return isStaticState; };

    // Inertia
    void calculateInertia(Shape& s);

    // Methods for calculating the delta position
    Vector2 semiImplicitEuler(float delta_time); 
    Vector2 forwardEuler(float delta_time); 
    Vector2 midPointMethod(float delta_time); 
    Vector2 rungeKutta4(float delta_time); 

    // For testing purpose
    void move(Vector2 delta);
    void rotate(float radiansDelta);

    std::string toStringFandV();

    virtual ~RigidBody() = default;  
};

#endif