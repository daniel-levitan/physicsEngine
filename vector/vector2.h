/**
 * File: vector.h 
 * --------------
 * This interface exports the Vector class, which represents a 
 * vector in the x-y plane.
 */

#ifndef _vector2_h
#define _vector2_h

#include <string>


class Vector2 {
private:
	float x;
	float y;

public:
    static const Vector2 Null; // Null vector declaration

	Vector2();
	Vector2(float xc, float yc);

	float getX() const;
	float getY() const;
	Vector2 getNormal() const;
	
	void setX(float xc);
	void setY(float yc);

	double length2();
	double length();
	void normalize();
	double dotProduct(const Vector2 other) const;
	
	void add(Vector2 other);
	void sub(Vector2 other);
	void scale(float scalar);
	float cross(Vector2 other);

	std::string toString();
	Vector2 copy();

	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const; 
	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
};

Vector2 add(const Vector2& v1, const Vector2& v2);
Vector2 sub(const Vector2& v1, const Vector2& v2);
Vector2 Scale(const Vector2& v1, float scalar);

#endif