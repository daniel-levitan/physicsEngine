#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include "./constants.h"
#include "./helper.h"
#include "../../vector/vector2.h"
#include "../../graphics/drawing.h"
#include "../../graphics/shape.h"
#include "../../graphics/circle.h"
#include "../../graphics/polygon.h"
#include "../../graphics/rectangle.h"


using namespace std;


int engine_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Mouse mouse;

int last_frame_time = 0;

Vector2 v1;
Vector2 v2;
Vector2 v3;
// Vector2 v1 = Vector2(0, 0); 
// Vector2 v2 = Vector2(WINDOW_WIDTH/4, WINDOW_HEIGHT/4);
// Vector2 v3 = Vector2(0, WINDOW_HEIGHT/4);
vector<Vector2> myVertices; // = {v1, v2, v3};
Polygon myPolygon; 
Circle myCircle;
Rectangle myRectangle;

int initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return FALSE;
	}

	return TRUE;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
	case SDL_QUIT: 
		engine_is_running = FALSE;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			engine_is_running = FALSE;
		break;
	}

	// get cursor position relative to window
    mouse.buttons = SDL_GetMouseState(&mouse.x, &mouse.y);    
    // printf("(%d, %d): %d\n", mouse.x, mouse.y, mouse.buttons);   
}

void setup() {	
	v1 = Vector2(50, 50);
    v2 = Vector2(WINDOW_WIDTH/4, WINDOW_HEIGHT/4);
    v3 = Vector2(50, WINDOW_HEIGHT/4);
    myVertices = {v1, v2, v3};

    // cout << v1 << ", " << v2 << ", " << v3 << endl;
    myPolygon = Polygon(renderer, myVertices);
    myCircle = Circle(renderer, Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), 20);
    myRectangle = Rectangle(renderer, 
    						Vector2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4), 
    						200, 80);     
}

void update() {
	/* Compute delta time */
	// float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // compute last time
	last_frame_time = SDL_GetTicks();
}

void render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

    myPolygon.draw();
    myCircle.draw();
    myRectangle.draw();
    SDL_RenderPresent(renderer);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	engine_is_running = initialize_window();

	setup();

	while (engine_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}


