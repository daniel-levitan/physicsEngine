#include <iostream>
#include <SDL.h>
#include "constants.h"
#include "helper.h"
#include "engine.h"
#include "../../vector/vector2.h"
// #include "../../graphics/drawing.h"
#include "../../graphics/shape.h"
#include "../../graphics/circle.h"
#include "../../graphics/polygon.h"
#include "../../graphics/rectangle.h"


using namespace std;

// Mouse mouse;
void setup(Engine& engine) {
    std::vector<Vector2> polygonVertices = {
	        Vector2(100, 100),  // Vertex 1
	        Vector2(200, 100),  // Vertex 2
	        Vector2(150, 200)   // Vertex 3
    };

    Polygon* polygon = new Polygon(polygonVertices);
    Rectangle *rect1 = new Rectangle(Vector2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4), 200, 80);   
	Circle* circle = new Circle(Vector2(400, 300), 50);

    engine.add_shape(polygon);
    engine.add_shape(rect1);
    engine.add_shape(circle);

}

int main() {
	try {
	    Engine engine;

	    setup(engine);

	    while (engine.is_game_running()) {
	        engine.input_processing();
	        engine.updating();
	        engine.rendering();
	    }

	} catch (const std::exception& e) {
    	std::cerr << "Error: " << e.what() << std::endl;
    	return 1;
	}
	return 0;
}


