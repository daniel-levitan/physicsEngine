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
public:
	/**
     * Constructor: Vector
     * Usage: Vector vector;
     *        Vector vector(xc, yc);
     * ------------------------------
     * Creates a vector object. The default constructor sets the 
     * coordinates to 0; the second form sets the coordinates to xc and yc.
  	 */
	Vector2();
	Vector2(float xc, float yc);

	/**
	 * Methods: getX, getY
	 * Usage: float x = vector.getX();
	 * --------------------------------
	 * These methods return the x and y coordinates of the vector.
	 */

	float getX() const;
	float getY() const;

	/**
	 * Method: length2
	 * Usage: double len2 = vector.length2();
	 * --------------------------------
	 * This method returns the length of the vector to the power of 2.
	 */

	double length2();
	
	/**
	 * Method: length
	 * Usage: double len = vector.length();
	 * --------------------------------
	 * This method returns the length of the vector.
	 */

	double length();

	/**
	 * Method: normalize
	 * Usage: vector.normalize();
	 * --------------------------------
	 * This method normalizes the vector.
	 */

	void normalize();

	/**
	 * Method: getNormal
	 * Usage: vector normal = vector.getNormal();
	 * --------------------------------
	 * This method returns the normal vector.
	 */

	Vector2 getNormal();

	/**
	 * Method: dotProduct
	 * Usage: double product = vector.dotProduct(Vector v2);
	 * --------------------------------
	 * This method returns the dotProduct with another vector.
	 */

	double dotProduct(Vector2 other);

	/**
	 * Method: toString
	 * Usage: string str = vector.toString();
	 * ---------------------------------------
	 * Returns a string representations of the Vector in the form (x, y).
	 * */
	std::string toString();

	/**
	 * Method: copy
	 * Usage: Vector new_vec = vector.copy();
	 * ---------------------------------------
	 * Returns a copy of the original vector.
	 * */

	Vector2 copy();

	/**
	 * Method: add
	 * Usage: Vector result = vector.add(Vector other);
	 * ---------------------------------------
	 * Add the coordinates of this vector and the other vector.
	 * */

	void add(Vector2 other);

	/**
	 * Method: sub
	 * Usage: Vector result = vector.sub(Vector other);
	 * ---------------------------------------
	 * Subtract the coordinates of this vector from the other vector.
	 * */

	void sub(Vector2 other);

	/**
	 * Method: scale
	 * Usage: vector.scale(float scalar);
	 * ---------------------------------------
	 * Scales the vector by scalar.
	 * */

	void scale(float scalar);

	// Method to return a new scaled vector (does not mutate the original)
    Vector2 Scale(float scalar);


	/**
	 * Method: cross
	 * Usage: vector.cross(Vector other);
	 * ---------------------------------------
	 * ???
	 * */

	float cross(Vector2 other);


	/**
	 *  Operators overload
	 * 
	 * 
	 * 
	 */
	bool operator==(const Vector2& other) const;
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const; 

	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);


private:
	float x;
	float y;
};

/**
 * Method: add
 * Usage: Vector result = add(Vector v1, Vector v2);
 * --------------------------------------------------
 * Add the coordinates of this vector and the other vector.
 * */
Vector2 add(const Vector2& v1, const Vector2& v2);

/**
 * Method: sub
 * Usage: Vector result = vector.sub(Vector v1, Vector v2);
 * ---------------------------------------------------------
 * Subtract the coordinates of this vector from the other vector.
 * */
Vector2 sub(const Vector2& v1, const Vector2& v2);


/**
 * Method: scale
 * Usage: Vector result = scale(Vector v1, float scalar);
 * ---------------------------------------------------------
 * Scales the vector by scalar.
 * */
Vector2 scale(const Vector2& v1, float scalar);

#endif