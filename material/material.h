#ifndef _material_h
#define _material_h

class Material {
protected:
    float bounce;
    float friction;

public:
    Material(float bounce, float friction) : bounce(bounce), friction(friction) {};
    Material() {
        bounce = 1.0;
        friction = 0.0;
    };

    float getBounce() { return bounce; };
    float getFriction() { return friction; };

};

#endif