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
	engine.initialization();
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


