#ifndef _material_h
#define _material_h

class Material {
protected:
    float restitution;
    float friction;

public:
    Material(float restitution, float friction) : restitution(restitution), friction(friction) {};
    Material() {
        restitution = 1.0f;
        friction = 0.0f;
    };

    float getRestitution() { return restitution; };
    float getFriction() { return friction; };

};

#endif