#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "../graphics/shape.h"
#include "../graphics/circle.h"
#include "../graphics/polygon.h"
#include "../graphics/rectangle.h"
#include "../collision/collision.h"
#include "../utils/utils.h"
#include "../../vector/vector2.h"

Engine::Engine() 
    : engine_is_running(false),       
      input_manager(std::make_unique<InputManager>()) // Create InputManager
{ 
	try {
    	graphics = std::make_unique<Graphics>(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window");
        engine_is_running = true;  // If no exception, graphics initialization is successful
        // initialization();

        f1 = false;
        f3 = false;
        debugMode = false;
        previousF3State = false;

        move_left = move_right = move_up = move_down = false;
        rotate_left = rotate_right = false;
        move_leftB = move_rightB = move_upB = move_downB = false;
        rotate_leftB = rotate_rightB = false;
        
        // last_frame_time = 0;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error initializing Graphics: " << e.what() << std::endl;
        // engine_is_running remains false if an exception is thrown
    }
}

void Engine::setupPlayers() {
    /* Now we are going to work with rigid bodies */
    Color whiteColor = {255, 255, 255};
    Color blueColor = {0, 0, 255};
    auto rectA = std::make_unique<Rectangle>(Vector2(300, 300), 200, 100, blueColor);
    auto rb1 = std::make_unique<RigidBody>(std::move(rectA), 40.0f, 1.0f, 0.0f);

    auto rectB = std::make_unique<Rectangle>(Vector2(400, 150), 200, 100, whiteColor);
    auto rb2 = std::make_unique<RigidBody>(std::move(rectB), 40.0f, 0.5f, 0.0f);
    
    auto rectC = std::make_unique<Rectangle>(Vector2(180, 150), 200, 100, whiteColor);
    auto rb3 = std::make_unique<RigidBody>(std::move(rectC), 40.0f, 0.5f, 0.0f);

    add_rigid_body(std::move(rb1));
    add_rigid_body(std::move(rb2));
    add_rigid_body(std::move(rb3));

    auto circleA = std::make_unique<Circle>(Vector2(600, 300), 60, whiteColor);
    auto rb4 = std::make_unique<RigidBody>(std::move(circleA), 40.0f, 1.0f, 0.0f);
    add_rigid_body(std::move(rb4));

    /*
    // auto circleA = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/2 + 0, WINDOW_HEIGHT/4), 70, whiteColor);
    auto circleA = std::make_unique<Circle>(Vector2(100, 300), 30, whiteColor);
    auto rb1 = std::make_unique<RigidBody>(std::move(circleA), 10.0f, 1.0f, 0.0f);

    auto circleB = std::make_unique<Circle>(Vector2(300, 300), 60, whiteColor);
    auto rb2 = std::make_unique<RigidBody>(std::move(circleB), 50.0f, 1.0f, 0.0f);
    */

    /*
    auto circleC = std::make_unique<Circle>(Vector2(500, 300), 50, whiteColor);
    auto rb3 = std::make_unique<RigidBody>(std::move(circleC), 10.0f, 1.0f, 0.0f);

    auto circleD = std::make_unique<Circle>(Vector2(700, 300), 50, whiteColor);
    auto rb4 = std::make_unique<RigidBody>(std::move(circleD), 10.0f, 1.0f, 0.0f);
    // auto circleC = std::make_unique<Circle>(Vector2(WINDOW_WIDTH/2 + 160, WINDOW_HEIGHT/4), 50, whiteColor);
    // auto rb3 = std::make_unique<RigidBody>(std::move(circleC), 10.0f, 0.8f, 0.0f);   
    */

    /*
    engine.add_rigid_body(std::move(rb4));
    engine.add_rigid_body(std::move(rb5));
    engine.add_rigid_body(std::move(rb6));
    */

    /*
    auto rectA = std::make_unique<Rectangle>(Vector2(200, 500), 200, 80, whiteColor);
    auto rb1 = std::make_unique<RigidBody>(std::move(rectA), 10.0f, 1.0f, 0.0f);

    auto rectB = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/4 + 20, WINDOW_HEIGHT/2), 80, 200, whiteColor);
    rectB->rotate(1.3);
    auto rb2 = std::make_unique<RigidBody>(std::move(rectB), 10.0f, 1.0f, 0.0f);
                                            
    engine.add_rigid_body(std::move(rb1));
    engine.add_rigid_body(std::move(rb2));
    */
    
    // Color green = {0, 255, 0};
    // auto staticRect = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 100), 50, 50, green);
    // auto staticRb = std::make_unique<RigidBody>(std::move(staticRect), 0.0f);
    // add_rigid_body(std::move(staticRb));
}

void Engine::setupWorldConstraints() {
    /* The following lines add the boundaries to the "world" */
    Color green = {0, 255, 0};
    auto rectLeftWall = std::make_unique<Rectangle>(Vector2(0 - 6, WINDOW_HEIGHT/2), 10, WINDOW_HEIGHT, green);
    auto rbLeftWall = std::make_unique<RigidBody>(std::move(rectLeftWall), 0.0f);
    add_rigid_body(std::move(rbLeftWall));

    auto rectRightWall = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH + 6, WINDOW_HEIGHT/2), 10, WINDOW_HEIGHT, green);
    auto rbRightWall = std::make_unique<RigidBody>(std::move(rectRightWall), 0.0f);
    add_rigid_body(std::move(rbRightWall));

    auto rectFloor = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT + 5), WINDOW_WIDTH, 10, green);
    auto rbFloor = std::make_unique<RigidBody>(std::move(rectFloor), 0.0f);
    add_rigid_body(std::move(rbFloor));

    auto rectCeiling = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, 0 - 6), WINDOW_WIDTH, 10, green);
    auto rbCeiling = std::make_unique<RigidBody>(std::move(rectCeiling), 0.0f);
    add_rigid_body(std::move(rbCeiling));
}



void Engine::setupScenario() {
    SDL_Color white = {255, 255, 255, 255};
    /* Text on screen */
    const std::string message = " ";
    auto text = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, 10), white);
    add_text(std::move(text));
    
    auto text1 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(WINDOW_WIDTH/2, 10), white);  
    add_text(std::move(text1));

    auto text2 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, WINDOW_HEIGHT - 70), white);
    add_text(std::move(text2));
}


bool Engine::initialization() {
    std::cout << "initialization called" << std::endl;
	last_frame_time = 0;
    
    setupPlayers();
    setupWorldConstraints();
    setupScenario();
	return true;
}

void Engine::input_processing() {
    input_manager->processInput(engine_is_running);

    if (input_manager->isKeyDown(SDLK_ESCAPE)) {
        std::cout << "Escape key is pressed!" << std::endl;
        engine_is_running = false;
    }

    if (input_manager->isKeyDown(SDLK_F1)) { f1 = true; }
    if (input_manager->isKeyUp(SDLK_F1)) { f1 = false; }
    
    // Debug Mode
    bool currentF3State = input_manager->isKeyDown(SDLK_F3);
    if (currentF3State && !previousF3State) {
        debugMode = !debugMode;     // F3 was just pressed, toggle the debug mode
    }
    // Update the previous state for the next frame
    previousF3State = currentF3State;

    
    // Arrow control (it may be figure A or player 1)
    if (input_manager->isKeyDown(SDLK_LEFT))   { move_left = true; }
    if (input_manager->isKeyUp(SDLK_LEFT))     { move_left = false; }
    if (input_manager->isKeyDown(SDLK_RIGHT))  { move_right = true; }
    if (input_manager->isKeyUp(SDLK_RIGHT))    { move_right = false; }
    if (input_manager->isKeyDown(SDLK_UP))     { move_up = true; }
    if (input_manager->isKeyUp(SDLK_UP))       { move_up = false; }
    if (input_manager->isKeyDown(SDLK_DOWN))   { move_down = true; }
    if (input_manager->isKeyUp(SDLK_DOWN))     { move_down = false; }
    if (input_manager->isKeyDown(SDLK_COMMA))  { rotate_left = true; }
    if (input_manager->isKeyUp(SDLK_COMMA))    { rotate_left = false; }
    if (input_manager->isKeyDown(SDLK_PERIOD)) { rotate_right = true; }
    if (input_manager->isKeyUp(SDLK_PERIOD))   { rotate_right = false; }

    // Arrow control (it may be figure A or player 1)
    if (input_manager->isKeyDown(SDLK_a))  { move_leftB = true; }
    if (input_manager->isKeyUp(SDLK_a))    { move_leftB = false; }
    if (input_manager->isKeyDown(SDLK_d))  { move_rightB = true; }
    if (input_manager->isKeyUp(SDLK_d))    { move_rightB = false; }
    if (input_manager->isKeyDown(SDLK_w))  { move_upB = true; }
    if (input_manager->isKeyUp(SDLK_w))    { move_upB = false; }
    if (input_manager->isKeyDown(SDLK_s))  { move_downB = true; }
    if (input_manager->isKeyUp(SDLK_s))    { move_downB = false; }
    if (input_manager->isKeyDown(SDLK_q))  { rotate_leftB = true; }
    if (input_manager->isKeyUp(SDLK_q))    { rotate_leftB = false; }
    if (input_manager->isKeyDown(SDLK_e))  { rotate_rightB = true; }
    if (input_manager->isKeyUp(SDLK_e))    { rotate_rightB = false; }

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
    
    for (const auto& rb : rigidBodies) {
        rb->getShape()->setOverlap(false);
    }

    manifolds.clear();
    

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

    RigidBody* rb1 = rigidBodies[0].get();
    RigidBody* rb2 = rigidBodies[1].get();
    
    // Shape A/Player 1
    if (move_left) { rb2->addForce(Vector2(-FORCE, 0)); } // movement_delta_x -= movement_speed * delta_time;
    if (move_right) { rb2->addForce(Vector2(FORCE, 0)); } // movement_delta_x += movement_speed * delta_time;
    if (move_up) { rb2->addForce(Vector2(0, -FORCE)); } // movement_delta_y -= movement_speed * delta_time; 
    if (move_down) { rb2->addForce(Vector2(0, FORCE)); } // movement_delta_y += movement_speed * delta_time; 
    // if (rotate_left) { rotate_deltaA -= rotate_speed * delta_time; }
    // if (rotate_right) { rotate_deltaA += rotate_speed * delta_time; }
    // Vector2 deltaA(movement_delta_x, movement_delta_y);

    // Shape B/Player 2
    if (move_leftB) { rb1->addForce(Vector2(-FORCE, 0)); }   // movement_delta_xB -= movement_speed * delta_time;
    if (move_rightB) { rb1->addForce(Vector2(FORCE, 0)); }   // movement_delta_xB += movement_speed * delta_time;
    if (move_upB) { rb1->addForce(Vector2(0, -FORCE)); }   // movement_delta_yB -= movement_speed * delta_time;
    if (move_downB) { rb1->addForce(Vector2(0, FORCE)); }   // movement_delta_yB += movement_speed * delta_time;
    // if (rotate_leftB) { rotate_deltaB -= rotate_speed * delta_time; }
    // if (rotate_rightB) { rotate_deltaB += rotate_speed * delta_time; }
    // Vector2 deltaB(movement_delta_xB, movement_delta_yB);

    if (debugMode) {
        std::string str = rb2->toStringFandV();
        text2->setMessage(str);
    } else {
        std::string str = " ";
        text2->setMessage(str);
    }


    // Update bodies(shapes) positions
    for (const auto& rb : rigidBodies) {
        gravitationalForce = Vector2(0, rb->getMass() * DEFAULT_GRAVITY);
        rb->addForce(gravitationalForce);
        rb->update(delta_time);

        // if (Collision::checkFloorCollision(*rb->getShape(), WINDOW_HEIGHT))
            // rb->setVelocity(Scale(rb->getVelocity(), -1 * rb->getMaterial()->getBounce()));

        // if (std::abs(rb->getVelocity().getY()) < 1.0f) {
        //     rb->setVelocity(Vector2::Null);            
        // }
    }


     for (size_t i = 0; i < rigidBodies.size(); i++) {
        for (size_t j = 0; j < rigidBodies.size(); j++) {
            if (rigidBodies[i] == rigidBodies[j]) continue;
 
            // bool flag = false;
            auto result = Collision::collisionDetection(*rigidBodies[i].get(), *rigidBodies[j].get());
            // auto result = Collision::checkCollision(*rigidBodies[i]->getShape(), *rigidBodies[j]->getShape());
            if (result) {
                // flag = true;                
                Collision::positionCorrection(*rigidBodies[i].get(), *rigidBodies[j].get(), *result.get());
                Collision::resolveCollision(*rigidBodies[i].get(), *rigidBodies[j].get(), *result.get());

                if (debugMode) {
                    std::string str = result->toString();
                    text1->setMessage(str);
                }

                manifolds.push_back(std::move(result));
            }

            // rigidBodies[i]->getShape()->setOverlap(flag | rigidBodies[i]->getShape()->getOverlap());
            // rigidBodies[j]->getShape()->setOverlap(flag | rigidBodies[j]->getShape()->getOverlap());
        }
    }


    last_frame_time = SDL_GetTicks();
}

void Engine::rendering() {
	graphics->clearScreen(0, 0, 0, 255);  // Clear screen using the Graphics object

    for (const auto& shape : shapes) {
        shape->draw(graphics->getRenderer());  // Use Graphics renderer to draw shapes
    }

    /*
    for (const auto& manifold : manifolds) {
        manifold->draw(graphics->getRenderer());  // Draw the manifold (collision details)
    } 
    */  

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
