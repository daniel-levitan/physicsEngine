#include <iostream>
#include <SDL.h>
#include "constants.h"
#include "helper.h"
#include "engine.h"
#include "../../vector/vector2.h"
#include "../../graphics/shape.h"
#include "../../graphics/circle.h"
#include "../../graphics/polygon.h"
#include "../../graphics/rectangle.h"
#include "../../graphics/line.h"

using namespace std;

// Mouse mouse;
void setup(Engine& engine) {
	// Triângulo
    // std::vector<Vector2> polygonVertices = {
	//         Vector2(100, 100),  // Vertex 1
	//         Vector2(200, 100),  // Vertex 2
	//         Vector2(150, 200)   // Vertex 3
    // };

	// Polígono
    // std::vector<Vector2> polygonVertices2 = {
	//         Vector2(100, 100),
	//         Vector2(100, 200),
	//         Vector2(500, 150),
	//         Vector2(150, 100)
    // };

    // auto polygon = std::make_unique<Polygon>(polygonVertices2);	
	// auto rectangle = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4), 200, 80);
    auto circleA = std::make_unique<Circle>(Vector2(400, 300), 50);
    auto circleB = std::make_unique<Circle>(Vector2(200, 200), 50);

	// Vector2 start(500, 500);
	// Vector2 end(600, 500);
    // auto line = std::make_unique<Line>(start, end);
    
    // Vector2 rotated = line->rotateAroundPoint(end, start, 30 * M_PI / 180);
    // auto line2 = std::make_unique<Line>(start, rotated);
    // engine.add_shape(std::move(line2));  

	// engine.add_shape(std::move(rectangle));  // Use move semantics
	// engine.add_shape(std::move(polygon));  // Use move semantics
	engine.add_shape(std::move(circleA));  
	engine.add_shape(std::move(circleB));  

	// engine.add_shape(std::move(line));  
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


