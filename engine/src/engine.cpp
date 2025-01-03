#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "../graphics/shape.h"
#include "../graphics/circle.h"
#include "../graphics/polygon.h"
#include "../collision/collision.h"
// #include "../collision/manifold.h"
#include "../utils/utils.h"

Engine::Engine() 
    : engine_is_running(false),       
      input_manager(std::make_unique<InputManager>()) // Create InputManager
 { 
	try {
    	graphics = std::make_unique<Graphics>(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window");
        engine_is_running = true;  // If no exception, graphics initialization is successful

        f1 = f2 = f3 = false;
        debugMode = false;
        previousF2State = previousF3State = false;

        currentF2State = false;
        collisionMode = CollisionMode::PIXEL;

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
    // bool currentF2State = input_manager->isKeyDown(SDLK_F2);
    // int number_collision_modes = 2;
    
    currentF2State = input_manager->isKeyDown(SDLK_F2);
    if (currentF2State && !previousF2State) {
        f2 = true;
        
        // As of now, I'm only working with one mode
        // int collisionModeCount = static_cast<int>(CollisionMode::COUNT);
        // collisionMode = static_cast<CollisionMode>((static_cast<int>(collisionMode) + 1) % collisionModeCount);
    }
    // Update the previous state for the next frame
    previousF2State = currentF2State;

    if (input_manager->isKeyUp(SDLK_F2)) {
        f2 = false;
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
    // float movement_speed = 150.0f;  
    float movement_speed = 1.0f * MOVEMENT_SPEED;  
    float movement_delta_x = 0.0f, movement_delta_y = 0.0f;
    float movement_delta_xB = 0.0, movement_delta_yB = 0.0f;

    // float rotate_speed = 15.0f; 
    float rotate_speed = 1.0f * ROTATION_SPEED;
    float rotate_delta = 0.0f, rotate_deltaB = 0.0f;

    // Circle* circleA = dynamic_cast<Circle*>(shapes[0].get());
    // Rectangle* rect = dynamic_cast<Rectangle*>(shapes[0].get());
    // Circle* circleB = dynamic_cast<Circle*>(shapes[1].get());
    // Polygon* pentagon = dynamic_cast<Polygon*>(shapes[0].get());
    // Polygon* triangle = dynamic_cast<Polygon*>(shapes[1].get());
    // Polygon* rect1 = dynamic_cast<Rectangle*>(shapes[0].get());

    // Shapes recovery
    // Polygon* rect1 = dynamic_cast<Rectangle*>(shapes[0].get());
    // Polygon* rect2 = dynamic_cast<Rectangle*>(shapes[1].get());
    // Polygon* shape1 = dynamic_cast<Rectangle*>(shapes[0].get());
    Polygon* shape1 = dynamic_cast<Polygon*>(shapes[0].get());
    Polygon* shape2 = dynamic_cast<Polygon*>(shapes[1].get());
    // Polygon* shape2 = dynamic_cast<Rectangle*>(shapes[1].get());

    // Circle* circleA = dynamic_cast<Circle*>(shapes[0].get());
    // Circle* circleB = dynamic_cast<Circle*>(shapes[1].get());

    // Text recovery
    Text* text = texts[0].get();
    Text* text1 = texts[1].get();
    Text* text2 = texts[2].get();
    text1->setDirectionRightToLeft();
    if (debugMode) {
        text->setMessage("Debugging");
    } else {
        text->setMessage("Not debugging");
        text1->setMessage(" ");
    }

    // We need to set overlap to false every update so we can
    // freshly detect the overlap condition
    for (const auto& shape : shapes) {
        shape->setOverlap(false);
    }


    // Manifold* circleCollision = Collision::checkCircleCircle(*circleA, *circleB);
    // std::unique_ptr<Manifold> circleCollision = Collision::checkCircleCircle(*circleA, *circleB);
    // auto circleCollision = std::make_unique<Manifold>(Collision::checkCircleCircle(*circleA, *circleB));

    // Checking for circle collisions
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
    manifolds.clear();
    
    if (collisionMode == CollisionMode::PIXEL) {
        // Checking for polygons collisions mode 0    
        for (size_t i = 0; i < shapes.size(); i++) {
            for (size_t j = 0; j < shapes.size(); j++) {
                if (shapes[i] == shapes[j]) continue;

                bool result = Collision::checkCollision(*shapes[i], *shapes[j]);
                if (result) {
                    shapes[i]->setColor(red);
                    shapes[j]->setColor(red);
                } else {
                    shapes[i]->setColor(white);
                    shapes[j]->setColor(white);
                }
                // Testing shape types
                // if (shapes[i]->getType() == "Circle" and shapes[j]->getType() != "Circle") {
                //     auto circ = dynamic_cast<Circle*>(shapes[i].get());
                //     auto pol = dynamic_cast<Polygon*>(shapes[j].get());
                //     bool result = Collision::checkCirclePolygon(*circ, *pol);
                //     if (result) {
                //         circ->setColor(red);
                //         pol->setColor(red);

                //         // std::string str = "Circle vs Polygon";
                //         // text1->setMessage(str);                        
                //     } else {
                //         circ->setColor(white);
                //         pol->setColor(white);
                //     }

                    
                // }

                // According to the order, so far I won't get here. Revert and test
                // if (shapes[j]->getType() == "Circle" and shapes[j]->getType() != "Circle") {
                    // std::cout << "Pol Circle" << std::endl;
                    // circ auto = 
                    // checkCirclePolygon(*circ, Polygon&pol);


                    // std::string str = "Circle vs Polygon";
                    // text1->setMessage(str);                    
                // }
                

                /*

                // Polygon vs Polygon 
                auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
                auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());

                // auto rect1 = dynamic_cast<Polygon*>(shapes[i].get());
                // auto rect2 = dynamic_cast<Polygon*>(shapes[j].get());
                
                // auto result = Collision::checkPolygonPolygon(*rect1, *rect2);
                auto result = Collision::checkPolygonPolygon(*polygon1, *polygon2);
                if (result) {
                    // Vector2 diff = polygon2->getCentroid() - polygon1->getCentroid();
                    // bool polygon1IsPusher = result->getNormal().dotProduct(diff) > 0;

                    polygon1->setColor(red);
                    polygon2->setColor(red);

                    Vector2 push;
                    push = Scale(Scale(result->getNormal(), -1), result->getDepth() * 0.4999);
                    polygon2->move(push);
    
                    push = Scale(Scale(result->getNormal(), -1), result->getDepth() * -0.4999);                    
                    polygon1->move(push);   
    
                    if (debugMode) {
                        std::string str = result->toString();
                        text1->setMessage(str);
                    }
                    manifolds.push_back(std::move(result));
                } else {                    
                    polygon1->setColor(white);
                    polygon2->setColor(white);
                    // manifolds.clear();
                }
                */
            }
        }
    } else if (collisionMode == CollisionMode::SAT) {
        for (size_t i = 0; i < shapes.size(); i++) {
            for (size_t j = i + 1; j < shapes.size(); j++) {
                // auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
                // auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());
                auto rect1 = dynamic_cast<Polygon*>(shapes[i].get());
                auto rect2 = dynamic_cast<Polygon*>(shapes[j].get());

                bool result = Collision::checkPolygonPolygonSAT(*rect1, *rect2);                
                // bool result = Collision::checkPolygonPolygonSAT(*polygon1, *polygon2);                
                rect1->setOverlap(result | rect1->getOverlap());
                // polygon1->setOverlap(result | polygon1->getOverlap());

            }
        } 
    } else if (collisionMode == CollisionMode::DIAGONAL) {
        for (size_t i = 0; i < shapes.size(); i++) {
            for (size_t j = i + 1; j < shapes.size(); j++) {
                // auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
                // auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());

                auto rect1 = dynamic_cast<Polygon*>(shapes[i].get());
                auto rect2 = dynamic_cast<Polygon*>(shapes[j].get());

                bool result = Collision::checkPolygonPolygonDIAG(*rect1, *rect2);
                // bool result = Collision::checkPolygonPolygonDIAG(*polygon1, *polygon2);
                rect1->setOverlap(result | rect1->getOverlap());
                // rect1->setOverlap(result | polygon1->getOverlap());
                // polygon1->setOverlap(result | polygon1->getOverlap());
            }
        }
    } else if (collisionMode == CollisionMode::DIAGONAL_RES) {
        for (size_t i = 0; i < shapes.size(); i++) {
            for (size_t j = i + 1; j < shapes.size(); j++) {
                // auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
                // auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());

                auto rect1 = dynamic_cast<Polygon*>(shapes[i].get());
                auto rect2 = dynamic_cast<Polygon*>(shapes[j].get());

                bool result = Collision::resPolygonPolygonDIAG(*rect1, *rect2);
                // bool result = Collision::resPolygonPolygonDIAG(*polygon1, *polygon2);
                // polygon1->setOverlap(result | polygon1->getOverlap());
                rect1->setOverlap(result | rect1->getOverlap());

            }
        }
    } else if (collisionMode == CollisionMode::SAT_RES) {
        for (size_t i = 0; i < shapes.size(); i++) {
            for (size_t j = i + 1; j < shapes.size(); j++) {
                // auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
                // auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());

                auto rect1 = dynamic_cast<Polygon*>(shapes[i].get());
                auto rect2 = dynamic_cast<Polygon*>(shapes[j].get());

                auto result = Collision::resPolygonPolygonSAT(*rect1, *rect2);
                // float overlap = Collision::resPolygonPolygonSAT(*polygon1, *polygon2);
                bool flag;
                flag = (result > 0) ? true : false;
                // polygon1->setOverlap(flag | polygon1->getOverlap());
                rect1->setOverlap(flag | rect1->getOverlap());

                if (result) {

                    Vector2 push;
                    push = Scale(Scale(result->getNormal(), -1), result->getDepth() * 1);
                    rect1->move(push);   
    
                    if (debugMode) {
                        std::string str = result->toString();
                        text1->setMessage(str);
                    }

                    manifolds.push_back(std::move(result));                    
                } else {
                    
                }
                // if (overlap) {
                    // if (debugMode) {
                        // std::string str = "Overlap: " + Utils::to_string_with_precision(overlap, 2);
                        // text1->setMessage(str);
                    // }
                // }
            }
        } 
    } else if (collisionMode == CollisionMode::TEST) {
        for (size_t i = 0; i < shapes.size(); i++) {
            for (size_t j = i + 1; j < shapes.size(); j++) {
                // auto polygon1 = dynamic_cast<Polygon*>(shapes[i].get());
                // auto polygon2 = dynamic_cast<Polygon*>(shapes[j].get());

                auto rect1 = dynamic_cast<Polygon*>(shapes[i].get());
                auto rect2 = dynamic_cast<Polygon*>(shapes[j].get());

                auto result = Collision::resIntersectPolygons(*rect1, *rect2);
                // auto result = Collision::resIntersectPolygons(*polygon1, *polygon2);
                bool flag = (result) ? true : false;
                // polygon1->setOverlap(flag | polygon1->getOverlap());
                rect1->setOverlap(flag | rect1->getOverlap());

                if (result) {

                    Vector2 push;
                    // push = Scale(Scale(result->getNormal(), 1), result->getDepth() * .5);
                    // polygon2->move(push);
                    // rect2->move(push);
    
                    push = Scale(Scale(result->getNormal(), -1), result->getDepth() * 1);
                    rect1->move(push);   
                    // // polygon1->move(push);   
    
                    if (debugMode) {
                        std::string str = result->toString();
                        text1->setMessage(str);
                    }

                    manifolds.push_back(std::move(result));                    
                } else {
                    
                }
            }
        }
    }
    

    // Menu/special keys  

    // Reset position  
    if (f1) {
        for (const auto& shape : shapes) {
            shape->resetPosition();
            shape->setOverlap(false);
        }  
    }

    // Collision mode - f2
    if (f2) {
        Color white = {255, 255, 255};
        for (const auto& shape : shapes) 
            shape->setColor(white);
    }

    std::string str = "Mode: ";
    switch (collisionMode) {
    case CollisionMode::PIXEL:
        str += "Pixel Collision";
        break;
    case CollisionMode::SAT:
        str += "SAT checking";
        break;
    case CollisionMode::DIAGONAL:
        str += "Diagonal checking";
        break;
    case CollisionMode::DIAGONAL_RES:
        str += "Diagonal RESOLUTION";
        break;
    case CollisionMode::SAT_RES:
        str += "SAT RESOLUTION";
        break;
    case CollisionMode::TEST:
        str += " == TESTING == ";
        break;    
    default:
        // Handle unexpected cases
        break;
}


    text2->setMessage(str);
    
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
    // Update movement and rotation
    for (const auto& shape : shapes) {
        if ((deltaA != Vector2(0,0)) || (deltaB != Vector2(0,0)) || rotate_delta || rotate_deltaB) {

            
            // if (circleB == &*shape) {
            if (shape2 == &*shape) {
                shape->move(deltaA);
                shape->rotate(rotate_delta);
            }

            // if (shape2 == &*shape) {
            //     shape->move(deltaB);
            //     shape->rotate(rotate_deltaB);
            // }

            // if (circleA == &*shape) {
            if (shape1 == &*shape) {
                shape->move(deltaB);
                shape->rotate(rotate_deltaB);
            }
            // if (circleB == &*shape) {
            // if (triangle == &*shape) {
            //     shape->move(deltaB);
            //     shape->rotate(rotate_deltaB);
            // }
            // if (pentagon == &*shape) {               
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

    for (const auto& text : texts) {
        text->render(graphics->getRenderer());  // Draw the manifold (collision details)
    }   

    graphics->presentScreen();  // Present the screen using the Graphics object
}

bool Engine::is_game_running() {
	return engine_is_running;
}

void Engine::add_shape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape)); // Store shapes using move semantics
}

void Engine::add_text(std::unique_ptr<Text> text) {
    texts.push_back(std::move(text));
}


Engine::~Engine() {
}
