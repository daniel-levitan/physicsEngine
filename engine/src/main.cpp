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
	// auto rectangle = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4), 200, 80);
    // auto circleA = std::make_unique<Circle>(Vector2(400, 300), 30);
    // auto circleB = std::make_unique<Circle>(Vector2(200, 200), 50);
	// engine.add_shape(std::move(rectangle));  
	// engine.add_shape(std::move(circleA));  
	// engine.add_shape(std::move(circleB));  

    // std::vector<Vector2> pentagon = {Vector2(550.0-25, 300.0),
	// 							  	 Vector2(475.0, 427.5),
	// 								 Vector2(325.0, 427.5),
	// 								 Vector2(250.0+25, 300.0),
	// 								 Vector2(400, 195)};
    // auto polygon = std::make_unique<Polygon>(pentagon);
    // engine.add_shape(std::move(polygon));

    // std::vector<Vector2> triangle = {	    
	//     Vector2(180+20, 150),
	//     Vector2(120-20, 150),
	//     Vector2(150, 20)
	// };
	// auto tri = std::make_unique<Polygon>(triangle);
	// engine.add_shape(std::move(tri));


    // auto line = std::make_unique<Line>(Vector2(325.0, 427.5),Vector2(250.0+25, 300.0));
	
    // line->setColor(color);
	// engine.add_shape(std::move(line));  
	Color whiteColor = {255, 255, 255};
	auto rect1 = std::make_unique<Rectangle>(Vector2(3*WINDOW_WIDTH/4, 1 * WINDOW_HEIGHT/4), 200, 80, whiteColor);
	auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4), 200, 80, whiteColor);	
	engine.add_shape(std::move(rect1));
	engine.add_shape(std::move(rect2));


	SDL_Color white = {255, 255, 255, 255};
	const std::string message = " ";
	auto text = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message,
										24, Vector2(10, 10), white);
	engine.add_text(std::move(text));

	auto text1 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message,
		 								24, Vector2(WINDOW_WIDTH/2, 10), white);	
	engine.add_text(std::move(text1));
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


