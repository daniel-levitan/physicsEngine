#include <iostream>
#include <SDL.h>
#include "constants.h"
#include "helper.h"
#include "engine.h"
#include "../../vector/vector2.h"
#include "../../graphics/shape.h"
#include "../../graphics/circle.h"
#include "../../graphics/polygon.h"
#include "../../graphics/polygonFactory.h"
#include "../../graphics/rectangle.h"
#include "../../graphics/line.h"

using namespace std;

// Mouse mouse;
void setup(Engine& engine) {
	Color whiteColor = {255, 255, 255};

	// Rectangles
	// auto rect1 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2), 200, 80, whiteColor);
	// engine.add_shape(std::move(rect1));
	// auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2 + 120, WINDOW_HEIGHT/2), 200, 80, whiteColor);
	// engine.add_shape(std::move(rect2));

	// Circles
    // auto circleA = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/4, WINDOW_HEIGHT/4), 50, whiteColor, "Circle");
    // auto circleA = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/4), 50, whiteColor);
	// engine.add_shape(std::move(circleA));  
    // auto circleB = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/2 + 60, WINDOW_HEIGHT/4), 50, whiteColor);
	// engine.add_shape(std::move(circleB));  

	// Line tests
    // auto line = std::make_unique<Line>(Vector2(325.0, 427.5),Vector2(250.0+25, 300.0));	
    // line->setColor(color);
	// engine.add_shape(std::move(line));  
	
	Vector2 pos1 = Vector2(200, 200);
	auto pent = PolygonFactory::createPolygon(5, pos1, 80, whiteColor);
	engine.add_shape(std::move(pent));
	Vector2 pos2 = Vector2(400, 400);
	auto tri = PolygonFactory::createPolygon(3, pos2, 50, whiteColor);
	engine.add_shape(std::move(tri));
	
	
	// Text on screen
	SDL_Color white = {255, 255, 255, 255};
	const std::string message = " ";
	auto text = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, 10), white);
	engine.add_text(std::move(text));
	auto text1 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(WINDOW_WIDTH/2, 10), white);	
	engine.add_text(std::move(text1));
	auto text2 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, WINDOW_HEIGHT - 50), white);
	engine.add_text(std::move(text2));
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


