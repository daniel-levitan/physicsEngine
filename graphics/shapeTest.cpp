#include <iostream>
#include "circle.h"
#include "../vector/vector2.h"

using namespace std;

int main(){
	Vector2 v1 = Vector2(100, 100);
	Vector2 v2 = Vector2(200, 200);
	cout << v1 << " " << v2 << endl;

	vector<Vector2> my_vertices = {v1, v2};

	Shape myShape(my_vertices);
	Circle myCircle(v1, 5);
	myCircle.draw();

	cout << myCircle.getRadius() << endl;
	cout << myCircle.getCenter() << endl;

	
}