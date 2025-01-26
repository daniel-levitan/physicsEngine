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
        forceAccumulator = Vector2(0, 0);
        velocityAccumulator = Vector2(0, 0);
        invertedMass = 0.0f;

        if (mass > 0) {
            invertedMass = 1.0f / mass;
        } 
    };

    void addForce(Vector2 force);
    void setForce(Vector2 force);
    void addVelocity(Vector2 velocity);
    void setVelocity(Vector2 velocity);

    void draw(SDL_Renderer* renderer);
    // std::unique_ptr<Shape> getShape();
    Shape* getShape() { return shape.get(); }


    Vector2 integrate(float delta_time);
    void update(float delta_time);

    void log();

    // For testing purpose
    void move(Vector2 delta);
    void rotate(float radiansDelta);

    std::string toStringFandV();

    virtual ~RigidBody() = default;  

};

#endif