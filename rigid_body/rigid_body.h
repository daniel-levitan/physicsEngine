#ifndef _rigid_body_h
#define _rigid_body_h

#include "../graphics/shape.h"
#include "../vector/vector2.h"

class RigidBody {
protected:
    std::unique_ptr<Shape> shape;
    float mass;
    float invertedMass;

    Vector2 forceAccumulator; 
    Vector2 velocityAccumulator;


public:
    RigidBody(std::unique_ptr<Shape> shape, float mass) : shape(std::move(shape)), mass(mass) {
        forceAccumulator = Vector2::Null;
        velocityAccumulator = Vector2::Null;
        invertedMass = 0.0f;

        if (mass > 0) {
            invertedMass = 1.0f / mass;
        } else {
            mass = 0.0f;
        }
    };

    // Force and velocity methods
    void addForce(Vector2 force);
    void setForce(Vector2 force);
    void addVelocity(Vector2 velocity);
    void setVelocity(Vector2 velocity);
    Vector2 getVelocity() { return velocityAccumulator; };

    void draw(SDL_Renderer* renderer);
    Shape* getShape() { return shape.get(); }     // std::unique_ptr<Shape> getShape();

    // Methods for updating the position
    void update(float delta_time);
    Vector2 integrate(float delta_time);

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