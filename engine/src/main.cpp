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

	/* Now we are going to work with rigid bodies */
	Color whiteColor = {255, 255, 255};

	auto circleA = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/4), 50, whiteColor);
	auto circleB = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/2 + 60, WINDOW_HEIGHT/4), 50, whiteColor);
	auto rb1 = std::make_unique<RigidBody>(std::move(circleA), 10.0f); /* RigidBody rb(std::unique_ptr<Shape>(std::move(circle)), 10.0f); */
	auto rb2 = std::make_unique<RigidBody>(std::move(circleB), 10.0f);	
	
	engine.add_rigid_body(std::move(rb1));
	engine.add_rigid_body(std::move(rb2));
		
	/* Text on screen */
	SDL_Color white = {255, 255, 255, 255};
	const std::string message = " ";
	auto text = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, 10), white);
	engine.add_text(std::move(text));
	
	auto text1 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(WINDOW_WIDTH/2, 10), white);	
	engine.add_text(std::move(text1));

	auto text2 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, WINDOW_HEIGHT - 70), white);
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


