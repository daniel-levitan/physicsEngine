#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "../graphics/shape.h"
#include "../graphics/circle.h"
#include "../graphics/polygon.h"
#include "../graphics/rectangle.h"
#include "../collision/collision.h"
#include "../utils/utils.h"

Engine::Engine() 
    : engine_is_running(false),       
      input_manager(std::make_unique<InputManager>()) // Create InputManager
{ 
	try {
    	graphics = std::make_unique<Graphics>(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window");
        engine_is_running = true;  // If no exception, graphics initialization is successful

        f1 = f3 = false;
        debugMode = false;
        previousF3State = false;

        move_left = move_right = move_up = move_down = false;
        rotate_left = rotate_right = false;
        move_leftB = move_rightB = move_upB = move_downB = false;
        rotate_leftB = rotate_rightB = false;
        
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

    if (input_manager->isKeyDown(SDLK_F1)) {
        f1 = true;
    }
    if (input_manager->isKeyUp(SDLK_F1)) {
        f1 = false;
    }
    
    // Debug Mode
    bool currentF3State = input_manager->isKeyDown(SDLK_F3);
    if (currentF3State && !previousF3State) {
        // F3 was just pressed, toggle the debug mode
        debugMode = !debugMode;
    }
    // Update the previous state for the next frame
    previousF3State = currentF3State;

    
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
    
    // Calculating deltas
    // float movement_speed = 1.0f * MOVEMENT_SPEED;  
    // float movement_delta_x = 0.0f, movement_delta_y = 0.0f;
    // float movement_delta_xB = 0.0, movement_delta_yB = 0.0f;

    // float rotate_speed = 1.0f * ROTATION_SPEED;
    // float rotate_deltaA = 0.0f, rotate_deltaB = 0.0f;

    // Text recovery
    Text* text = texts[0].get();
    Text* text1 = texts[1].get();
    Text* text2 = texts[2].get(); // Bottow left

    text1->setDirectionRightToLeft();
    if (debugMode) {
        text->setMessage("Debugging");
    } else {
        text->setMessage("Not debugging");
        text1->setMessage(" ");
    }

    manifolds.clear();
    
    /* Collision check */
    // for (size_t i = 0; i < shapes.size(); i++) {
    //     for (size_t j = 0; j < shapes.size(); j++) {
    //         if (shapes[i] == shapes[j]) continue;

    //         auto result = Collision::checkCollision(*shapes[i], *shapes[j]);
    //         bool flag = false;
    //         if (result)
    //             flag = true;
    //         shapes[i]->setOverlap(flag | shapes[i]->getOverlap());
    //         shapes[j]->setOverlap(flag | shapes[j]->getOverlap());

    //         if (result) {

    //             Vector2 push;
    //             push = Scale(Scale(result->getNormal(), -1), result->getDepth() * 0.4999);
    //             shapes[i]->move(push);   
    //             push = Scale(Scale(result->getNormal(), -1), result->getDepth() * -0.4999);
    //             shapes[j]->move(push);   

    //             if (debugMode) {
    //                 std::string str = result->toString();
    //                 text1->setMessage(str);
    //             }

    //             manifolds.push_back(std::move(result));                    
    //         }
    //     }
    // }
    

    // Menu/special keys  
    // Reset force and velocity  
    if (f1) {
        for (const auto& rb : rigidBodies) {
            rb->setForce(Vector2::Null);
            rb->setVelocity(Vector2::Null);

            rb->getShape()->resetPosition();
            rb->getShape()->setOverlap(false);
        }        
    }

    // RigidBody* rb1 = rigidBodies[0].get();
    RigidBody* rb2 = rigidBodies[1].get();
    
    // Shape A/Player 1
    if (move_left) {
        rb2->addForce(Vector2(-FORCE, 0));
        // movement_delta_x -= movement_speed * delta_time;
    }
    if (move_right) {
        rb2->addForce(Vector2(FORCE, 0));
        // movement_delta_x += movement_speed * delta_time;
    }
    if (move_up) {
        rb2->addForce(Vector2(0, -FORCE));
    //     movement_delta_y -= movement_speed * delta_time; 
    }
    if (move_down) {
        rb2->addForce(Vector2(0, FORCE));
    //     movement_delta_y += movement_speed * delta_time; 
    }
    // if (rotate_left) {
    //     rotate_deltaA -= rotate_speed * delta_time; 
    // }
    // if (rotate_right) {
    //     rotate_deltaA += rotate_speed * delta_time; 
    // }
    // Vector2 deltaA(movement_delta_x, movement_delta_y);
    std::string str = rb2->toStringFandV();
    text2->setMessage(str);


    // // Shape B/Player 2
    // if (move_leftB) {
    //     movement_delta_xB -= movement_speed * delta_time;
    // }
    // if (move_rightB) {
    //     movement_delta_xB += movement_speed * delta_time;
    // }
    // if (move_upB) {
    //     movement_delta_yB -= movement_speed * delta_time; 
    // }
    // if (move_downB) {
    //     movement_delta_yB += movement_speed * delta_time; 
    // }
    // if (rotate_leftB) {
    //     rotate_deltaB -= rotate_speed * delta_time; 
    // }
    // if (rotate_rightB) {
    //     rotate_deltaB += rotate_speed * delta_time; 
    // }
    // Vector2 deltaB(movement_delta_xB, movement_delta_yB);

    
    // === There must be a better way to do that ===    
    /*
    RigidBody* rb1 = rigidBodies[0].get();
    RigidBody* rb2 = rigidBodies[1].get();


    for (const auto& rb : rigidBodies) {
        if ((deltaA != Vector2(0,0)) || (deltaB != Vector2(0,0)) || rotate_deltaA || rotate_deltaB) {
            if (rb2 == &*rb) {
                rb->move(deltaA);
                rb->rotate(rotate_deltaA);
            }

            if (rb1 == &*rb) {
                rb->move(deltaB);
                rb->rotate(rotate_deltaB);
            }
        }
    }
    */

    for (const auto& rb : rigidBodies) {
        rb->update(delta_time);
    }
    // I can simply show in a text
    // rigidBodies[0].log();

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

    for (const auto& text : texts) {
        text->render(graphics->getRenderer());  // Draw the manifold (collision details)
    }  

    for (const auto& rb : rigidBodies) {
        (rb->getShape())->draw(graphics->getRenderer());
        // rb->draw(graphics->getRenderer());
    } 

    graphics->presentScreen();  // Present the screen using the Graphics object
}

bool Engine::is_game_running() {
	return engine_is_running;
}

void Engine::add_shape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape)); // Store shapes using move semantics
}

void Engine::add_rigid_body(std::unique_ptr<RigidBody> rigidBody) {
    rigidBodies.push_back(std::move(rigidBody)); // Store rigid body using move semantics
}

void Engine::add_text(std::unique_ptr<Text> text) {
    texts.push_back(std::move(text));
}


Engine::~Engine() {
}
