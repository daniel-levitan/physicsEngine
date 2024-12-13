#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "constants.h"

// Engine::Engine() 
// 		: graphics(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window")
// {	
// 	engine_is_running = true;
// 	last_frame_time = 0;
// }

Engine::Engine() : engine_is_running(false) { 
	try {
    	graphics = std::make_unique<Graphics>(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window");
        engine_is_running = true;  // If no exception, graphics initialization is successful
        last_frame_time = 0;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error initializing Graphics: " << e.what() << std::endl;
        // engine_is_running remains false if an exception is thrown
    }
}

bool Engine::initialization() {
	last_frame_time = 0;
	return true;
}

void Engine::input_processing() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	    switch (event.type) {
	    case SDL_QUIT:
	        engine_is_running = false;
	        break;
	    case SDL_KEYDOWN:
	        if (event.key.keysym.sym == SDLK_ESCAPE)
	            engine_is_running = false;
	        break;
    	}
	}

	// get cursor position relative to window
    // mouse.buttons = SDL_GetMouseState(&mouse.x, &mouse.y);    
    // printf("(%d, %d): %d\n", mouse.x, mouse.y, mouse.buttons);   
}

void Engine::updating() {
	/* Compute delta time */
	// float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // compute last time
    last_frame_time = SDL_GetTicks();
}

void Engine::rendering() {
	graphics->clearScreen(0, 0, 0, 255);  // Clear screen using the Graphics object

    for (const auto& shape : shapes) {
        shape->draw(graphics->getRenderer());  // Use Graphics renderer to draw shapes
    }
    graphics->presentScreen();  // Present the screen using the Graphics object

}

bool Engine::is_game_running() {
	return engine_is_running;
}

void Engine::add_shape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape)); // Store shapes using move semantics
}

Engine::~Engine() {
}
