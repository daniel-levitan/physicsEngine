#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "constants.h"
#include "../collision/collision.h"
#include "../collision/manifold.h"

Engine::Engine() 
    : engine_is_running(false),       
      input_manager(std::make_unique<InputManager>()) // Create InputManager
 { 
	try {
    	graphics = std::make_unique<Graphics>(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window");
        engine_is_running = true;  // If no exception, graphics initialization is successful
        move_left = move_right = move_up = move_down = false;
        rotate_left = rotate_right = false;
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
    input_manager->processInput(engine_is_running);

    if (input_manager->isKeyDown(SDLK_ESCAPE)) {
        std::cout << "Escape key is pressed!" << std::endl;
        engine_is_running = false;
    }
    
    // Arrow control (it may be figure A or player 1)
    if (input_manager->isKeyDown(SDLK_LEFT)) {
        move_left = true;
    }
    if (input_manager->isKeyUp(SDLK_LEFT)) {        
        move_left = false;
    }
    if (input_manager->isKeyDown(SDLK_RIGHT)) {
        move_right = true;
    }
    if (input_manager->isKeyUp(SDLK_RIGHT)) {        
        move_right = false;
    }
    if (input_manager->isKeyDown(SDLK_UP)) {
        move_up = true;
    }
    if (input_manager->isKeyUp(SDLK_UP)) {        
        move_up = false;
    }
    if (input_manager->isKeyDown(SDLK_DOWN)) {
        move_down = true;
    }
    if (input_manager->isKeyUp(SDLK_DOWN)) {        
        move_down = false;
    }
    if (input_manager->isKeyDown(SDLK_COMMA)) {
        rotate_left = true;
    }
    if (input_manager->isKeyUp(SDLK_COMMA)) {        
        rotate_left = false;
    }
    if (input_manager->isKeyDown(SDLK_PERIOD)) {
        rotate_right = true;
    }
    if (input_manager->isKeyUp(SDLK_PERIOD)) {        
        rotate_right = false;
    }

    // Arrow control (it may be figure A or player 1)
    if (input_manager->isKeyDown(SDLK_a)) {
        move_leftB = true;
    }
    if (input_manager->isKeyUp(SDLK_a)) {        
        move_leftB = false;
    }
    if (input_manager->isKeyDown(SDLK_d)) {
        move_rightB = true;
    }
    if (input_manager->isKeyUp(SDLK_d)) {        
        move_rightB = false;
    }
    if (input_manager->isKeyDown(SDLK_w)) {
        move_upB = true;
    }
    if (input_manager->isKeyUp(SDLK_w)) {        
        move_upB = false;
    }
    if (input_manager->isKeyDown(SDLK_s)) {
        move_downB = true;
    }
    if (input_manager->isKeyUp(SDLK_s)) {        
        move_downB = false;
    }
    if (input_manager->isKeyDown(SDLK_q)) {
        rotate_leftB = true;
    }
    if (input_manager->isKeyUp(SDLK_q)) {        
        rotate_leftB = false;
    }
    if (input_manager->isKeyDown(SDLK_e)) {
        rotate_rightB = true;
    }
    if (input_manager->isKeyUp(SDLK_e)) {        
        rotate_rightB = false;
    }

    // Example of checking for mouse button presses
    if (input_manager->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        std::cout << "Left mouse button is pressed!" << std::endl;   
    }

    // Example of getting mouse position
    int mouseX, mouseY;
    input_manager->getMousePosition(mouseX, mouseY);
    // std::cout << "Mouse position: (" << mouseX << ", " << mouseY << ")" << std::endl;
}

void Engine::updating() {
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
        
    float movement_speed = 150.0f;  
    float movement_delta_x = 0.0f, movement_delta_y = 0.0f;
    float movement_delta_xB = 0.0, movement_delta_yB = 0.0f;

    float rotate_speed = 15.0f; 
    float rotate_delta = 0.0f, rotate_deltaB = 0.0f;

    // auto *circleA = &shapes[0];
    // auto *circleB = &shapes[1];

    Circle* circleA = dynamic_cast<Circle*>(shapes[0].get());
    Circle* circleB = dynamic_cast<Circle*>(shapes[1].get());

    // Manifold* circleCollision = Collision::checkCircleCircle(*circleA, *circleB);
    // std::unique_ptr<Manifold> circleCollision = Collision::checkCircleCircle(*circleA, *circleB);
    // auto circleCollision = std::make_unique<Manifold>(Collision::checkCircleCircle(*circleA, *circleB));
    manifolds.clear();
    auto circleCollision = Collision::checkCircleCircle(*circleA, *circleB);

    if (circleCollision) {    
        Color newColor = {255, 0, 0};
        circleA->setColor(newColor);
        circleB->setColor(newColor);

        // If I use only this, the circle moves gently back
        // circleB->move(circleCollision->getNormal());
        
        Vector2 push = circleCollision->getNormal();
        push.scale(circleCollision->getDepth());
        circleB->move(push);

        // if (circleCollision)
            // manifolds.push_back(std::move(circleCollision));
    
        // float penetration = Collision::calculatePenetrationDepthOfCircles(*circleA, *circleB);

        // circleB->move(penetration);

        // std::cout << penetration << std::endl;
    } else {
        Color newColor = {255, 255, 255};
        circleA->setColor(newColor);
        circleB->setColor(newColor);
    }

    // Shape A/Player 1
    if (move_left) {
        movement_delta_x -= movement_speed * delta_time;
    }
    if (move_right) {
        movement_delta_x += movement_speed * delta_time;
    }
    if (move_up) {
        movement_delta_y -= movement_speed * delta_time; 
    }
    if (move_down) {
        movement_delta_y += movement_speed * delta_time; 
    }
    if (rotate_left) {
        rotate_delta -= rotate_speed * delta_time; 
    }
    if (rotate_right) {
        rotate_delta += rotate_speed * delta_time; 
    }
    Vector2 deltaA(movement_delta_x, movement_delta_y);


    // Shape B/Player 2
    if (move_leftB) {
        movement_delta_xB -= movement_speed * delta_time;
    }
    if (move_rightB) {
        movement_delta_xB += movement_speed * delta_time;
    }
    if (move_upB) {
        movement_delta_yB -= movement_speed * delta_time; 
    }
    if (move_downB) {
        movement_delta_yB += movement_speed * delta_time; 
    }
    if (rotate_leftB) {
        rotate_deltaB -= rotate_speed * delta_time; 
    }
    if (rotate_rightB) {
        rotate_deltaB += rotate_speed * delta_time; 
    }
    Vector2 deltaB(movement_delta_xB, movement_delta_yB);


    // There must be a better way to do that
    
    for (const auto& shape : shapes) {
        if (circleA == &*shape) {
            shape->move(deltaA);
            shape->rotate(rotate_delta);
        }
        if (circleB == &*shape) {
            shape->move(deltaB);
            shape->rotate(rotate_deltaB);
        }
    }

    last_frame_time = SDL_GetTicks();
}

void Engine::rendering() {
	graphics->clearScreen(0, 0, 0, 255);  // Clear screen using the Graphics object


    for (const auto& shape : shapes) {
        shape->draw(graphics->getRenderer());  // Use Graphics renderer to draw shapes
    }

    for (const auto& manifold : manifolds) {
        manifold->draw(graphics->getRenderer());  // Draw the manifold (collision details)
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
