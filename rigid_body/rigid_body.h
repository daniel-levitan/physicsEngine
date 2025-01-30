#ifndef _rigid_body_h
#define _rigid_body_h

#include "../graphics/shape.h"
#include "../vector/vector2.h"
#include "../material/material.h"

class RigidBody {
protected:
    std::unique_ptr<Shape> shape;
    float mass;
    float invertedMass;
    float inertia;
    float invertedInertia;
    float angularVelocity;

    Material material;

    Vector2 forceAccumulator; 
    Vector2 velocityAccumulator;

    Vector2 impulse;

    bool isKinematicState = false;

public:
    RigidBody(std::unique_ptr<Shape> shape, float mass = 1.0f, float bounce = 1.0f, float friction = 0.0f) : shape(std::move(shape)), mass(mass) {
        forceAccumulator = Vector2::Null;
        velocityAccumulator = Vector2::Null;
        angularVelocity = 0;
        
        material = Material(bounce, friction);
        // material = std::make_unique<Material>(bounce, friction);  

        if (mass > 0.00001) {
            invertedMass = 1.0f / mass;
        } else {
            mass = 0.0f;
            invertedMass = 0.0f;
            isKinematicState = true;
        }
        
        // calculateInertia(*this->shape);
        inertia = (*this->shape).calculateInertia(mass);
        if (inertia > 0.00001) {
            invertedInertia = 1 / inertia;
        } else {
            invertedInertia = 0;
        }
        
    };

    // Force and velocity methods
    void addForce(Vector2 force);
    void setForce(Vector2 force);
    void addVelocity(Vector2 velocity);
    void setVelocity(Vector2 velocity);
    Vector2 getVelocity() { return velocityAccumulator; };

    float getMass() { return mass; };
    float getInvertedMass() { return invertedMass; };

    float getAngularVelocity() { return angularVelocity; };

    void setImpulse(Vector2 impulse);
    Vector2 getImpulse() { return impulse;};

    void draw(SDL_Renderer* renderer);
    Shape* getShape() { return shape.get(); }     // std::unique_ptr<Shape> getShape();
    Material* getMaterial() { return &material; };
    // Material* getMaterial() { return material.get(); };

    // Methods for updating the position
    void update(float delta_time);
    Vector2 integrate(float delta_time);
    bool isKinematic() { return isKinematicState; };

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