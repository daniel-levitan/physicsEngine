#include <iostream>
#include "vector2.h"

using namespace std;

int main(){
	Vector2 vector1;
	// cout << vector1.toString() << endl;
	// cout << vector1 << endl;
	
	Vector2 vector2(3.0, 4.0);
	// cout << "Regular constructor ";
	// cout << "Parametrized constructor ";
	// cout << vector2.toString() << endl;
	// cout << vector2 << endl;	
	
	// cout << "length2 ";
	// cout << vector2.length2() << endl;

	// cout << "length ";
	// cout << vector2.length() << endl;

	// vector1 = vector2.copy(); 
	// cout << "vector1 " << vector1 << endl;

	// cout << "Normalized ";
	// vector2.normalize();
	// cout << vector2.toString() << endl;

	// cout << "Normal vector ";
	Vector2 vector3 = vector2.getNormal();
	// cout << vector3.toString() << endl;

	// cout << "Dot product ";
	// cout << vector2.dotProduct(vector3) << endl;

	// cout << "== ";
	// cout << (vector1 == vector2) << endl;

	Vector2 vector4(1.0, 2.0);
	// cout << "+ ";
	// Vector vector5 = vector2 + vector4;
	// cout << vector5 << endl;

	// vector5 = vector5 - vector4;
	// cout << "- ";
	// cout << vector5 << endl;

	// Vector vector6 = vector5.copy();
	// cout << "Copy ";
	// cout << vector6 << endl;


	cout << "Vector1 " << vector1 << endl;
	cout << "Vector2 " << vector2 << endl;
	cout << "Vector3 " << vector3 << endl;
	cout << "Vector4 " << vector4 << endl;
	// cout << "Vector5 " << vector5 << endl;
	// cout << "Vector6 " << vector6 << endl;
	
	// cout << "Add ";
	// vector1.add(vector4);
	// cout << vector1 << endl;

	// cout << "Sub ";
	// vector1.sub(vector4);
	// cout << vector1 << endl;

	// cout << (vector1 + vector3) << endl;

	cout << "vector3 + vector4 ";
	cout << (vector3 + vector4) << endl;

	cout << "vector2 - vector4 ";
	cout << (vector2 - vector4) << endl;

	cout << "Scale ";
	vector4.scale(1.5);
	cout << vector4 << endl;


	cout << "Vector2 " << vector2 << endl;
	cout << "Vector4 " << vector4 << endl;

	cout << "vector2 cross vector4: ";
	cout << vector2.cross(vector4) << endl;

	cout << "vector2 add vector4 " << add(vector2, vector4) << endl;
}