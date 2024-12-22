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

        f1 = f2 = f3 = false;
        debugMode = false;

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

    // Check collision mode
    if (input_manager->isKeyDown(SDLK_F2)) {
        f2 = true;
    }
    if (input_manager->isKeyUp(SDLK_F2)) {
        f2 = false;
    }

    // Debug Mode
    if (input_manager->isKeyDown(SDLK_F3)) {
        f3 = true;
        debugMode = !debugMode;
    }
    if (input_manager->isKeyUp(SDLK_F3)) {
        f3 = false;
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

    // Circle* circleA = dynamic_cast<Circle*>(shapes[0].get());
    // Rectangle* rect = dynamic_cast<Rectangle*>(shapes[0].get());
    // Circle* circleB = dynamic_cast<Circle*>(shapes[1].get());
    // Polygon* pentagon = dynamic_cast<Polygon*>(shapes[0].get());
    // Polygon* triangle = dynamic_cast<Polygon*>(shapes[1].get());

    Polygon* rect1 = dynamic_cast<Rectangle*>(shapes[0].get());
    Polygon* rect2 = dynamic_cast<Rectangle*>(shapes[1].get());
    
    // Manifold* circleCollision = Collision::checkCircleCircle(*circleA, *circleB);
    // std::unique_ptr<Manifold> circleCollision = Collision::checkCircleCircle(*circleA, *circleB);
    // auto circleCollision = std::make_unique<Manifold>(Collision::checkCircleCircle(*circleA, *circleB));

    /*
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
    */
            
    Color red = {255, 0, 0};
    Color white = {255, 255, 255};
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t j = 0; j < shapes.size(); j++) {
            if (shapes[i] == shapes[j]) continue;
                
            auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
            auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());
            
            auto result = Collision::checkPolygonPolygon(*polygon1, *polygon2);


            if (result) {
                // if (debugMode) 
                //     std::cout << *result << std::endl;
                // const Vector2 diff = polygon2->getCentroid() - polygon1->getCentroid();
                // bool polygon1IsPusher = result->getNormal().dotProduct(diff) > 0;
                // std::cout << *result << std::endl;
                // if (polygon1IsPusher) {
                //     std::cout << "Pol1 pushing" << std::endl;
                //     polygon2->setColor(red);
                // }
                // else {
                //     polygon1->setColor(red);
                //     std::cout << "Pol2 pushing" << std::endl;
                // }

                Vector2 push = Scale(Scale(result->getNormal(), -1), result->getDepth() * 0.5);
                polygon2->move(push);
                
                push = Scale(Scale(result->getNormal(), -1), result->getDepth() * -0.5);
                polygon1->move(push);

             
                
                // manifolds.push_back(std::move(result));
            } else {
                polygon1->setColor(white);
                polygon2->setColor(white);
                // manifolds.clear();
            }
                
        }
    }

    // auto &shapes = get_shapes();
    // size_t length = shapes.size();    
    // for (size_t i = 0; i < length; i++) {
    //     for (size_t j = 0; j < length; j++) {
    //         if (shapes[i] == shapes[j])
    //             continue;
    //         auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
    //         auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());

    //         bool result = Collision::checkPolygonPolygon(*polygon1, *polygon2);

    //         if (result) {
    //             polygon1->setColor(red);
    //         }
    //     }
    // }

    // Menu/special keys
    
    if (f1) {
        Vector2 position(3*WINDOW_WIDTH/4, 1 * WINDOW_HEIGHT/4);
        float width = 200;
        float height = 80;

        std::vector<Vector2> newVertices = {Vector2(position.getX() - width / 2, position.getY() - height / 2),
                                            Vector2(position.getX() + width / 2, position.getY() - height / 2),
                                            Vector2(position.getX() + width / 2, position.getY() + height / 2),
                                            Vector2(position.getX() - width / 2, position.getY() + height / 2)};
        rect1->setVertices(newVertices);

        Vector2 position2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4);
        newVertices = {Vector2(position2.getX() - width / 2, position2.getY() - height / 2),
                       Vector2(position2.getX() + width / 2, position2.getY() - height / 2),
                       Vector2(position2.getX() + width / 2, position2.getY() + height / 2),
                       Vector2(position2.getX() - width / 2, position2.getY() + height / 2)};
        rect2->setVertices(newVertices);
    }
    // auto rect1 = std::make_unique<Rectangle>(Vector2(3*WINDOW_WIDTH/4, 1 * WINDOW_HEIGHT/4), 200, 80);
    // auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/4, 3 * WINDOW_HEIGHT/4), 200, 80);

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
        if ((deltaA != Vector2(0,0)) || (deltaB != Vector2(0,0)) || rotate_delta || rotate_deltaB) {

            if (rect2 == &*shape) {
            // if (pentagon == &*shape) {
                shape->move(deltaB);
                shape->rotate(rotate_deltaB);
            }
            // if (circleA == &*shape) {
            //     shape->move(deltaA);
            //     shape->rotate(rotate_delta);
            // }
            // if (circleB == &*shape) {
            // if (triangle == &*shape) {
            //     shape->move(deltaB);
            //     shape->rotate(rotate_deltaB);
            // }
            if (rect1 == &*shape) {
            // if (pentagon == &*shape) {
                shape->move(deltaA);
                shape->rotate(rotate_delta);
            }
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
