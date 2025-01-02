/**
 * File: vector.cpp 
 * ----------------
 * This file implements the vector.h interface.
 */

#include <string>
#include <iostream>
#include "vector2.h"

using namespace std;

const Vector2 Vector2::Null(0.0f, 0.0f);

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(float xc, float yc) : x(xc), y(yc) {} 

float Vector2::getX() const { return x; }

float Vector2::getY() const { return y; }

void Vector2::setX(float xc) {
	x = xc;
}

void Vector2::setY(float yc) {
	y = yc;
}

double Vector2::length2() { 
	return x * x + y * y; 
	// return pow(x, 2) + pow(y, 2); 
}

double Vector2::length() { 
	return sqrt(length2());
}

void Vector2::normalize() {
	double len = length();
	if (len == 0) {
	    // Handle zero-length vector
        x = 0;
        y = 0;
        return;
    }
    x /= len;
    y /= len;
}

Vector2 Vector2::getNormal() const {
	return Vector2(y, -1 * x);
}

// double Vector2::dotProduct(Vector2 other) {
double Vector2::dotProduct(const Vector2 other) const {	
	return x * other.x + y * other.y;
}

Vector2 Vector2::copy() {
	return Vector2(x, y);
}

void Vector2::add(Vector2 other) {
	x += other.x;
	y += other.y;
}

void Vector2::sub(Vector2 other) {
	x -= other.x;
	y -= other.y;
}

void Vector2::scale(float scalar) {
	x *= scalar;
	y *= scalar;
}


float Vector2::cross(Vector2 other) {
	return x * other.y - y * other.x;
}

string Vector2::toString() {
	return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

bool Vector2::operator==(const Vector2& other) const {
    return (x == other.x && y == other.y);
}

bool Vector2::operator!=(const Vector2& other) const {
    return !(*this == other);
}

Vector2 Vector2::operator+(const Vector2& other) const {
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
	return Vector2(x - other.x, y - other.y);
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

Vector2 add(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.getX() + v2.getX(), v1.getY() + v2.getY());
}

Vector2 sub(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.getX() - v2.getX(), v1.getY() - v2.getY());
}

Vector2 Scale(const Vector2& v1, float scalar) {
    return Vector2(v1.getX() * scalar, v1.getY() * scalar);
}

Vector2 Normalize(Vector2& v) {
	Vector2 result(0,0);
	double len = v.length();
	if (len != 0) {
		result.setX(v.getX() / len);
    	result.setY(v.getY() / len);
	}
    return result;
}

// // Method to return a new scaled vector (does not mutate the original)
// Vector2 Vector2::scale(float scalar) {
// 	return Vector2(x * scalar, y * scalar);
// }

