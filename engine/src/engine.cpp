#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "constants.h"

Engine::Engine() {	
	engine_is_running = initialization();
	if (!engine_is_running) {
		// In case the program was not able to start, we return an error
		throw std::runtime_error("Engine initialization failed");
	}	
}

bool Engine::initialization() {
	last_frame_time = 0;
	
	// Initializing SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL:" << SDL_GetError() << std::endl;
		return false;
	}

	// Initializing window
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		std::cerr << "Error creating SDL Window:" << SDL_GetError() << std::endl;
		return false;
	}

	// Initializing renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Error creating SDL Renderer:" << SDL_GetError() << std::endl;
		return false;
	}
	
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

    // Iterate through all shapes and render them
    for (const Shape* shape : shapes) {
        shape->draw(renderer);  // Pass the renderer to each shape's draw method
    }

	SDL_RenderPresent(renderer);    
}

void Engine::cleanup() {
	if (renderer) 
		SDL_DestroyRenderer(renderer);

	if (window)
		SDL_DestroyWindow(window);

	SDL_Quit();	
}

bool Engine::is_game_running() {
	return engine_is_running;
}

void Engine::add_shape(Shape* shape) {
    shapes.push_back(shape);  // Add shape to the collection of shapes
}


Engine::~Engine() {
	cleanup();
	for (Shape* shape : shapes) {
        delete shape;  // Delete dynamically allocated shapes
    }
}
