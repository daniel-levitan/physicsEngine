#include <iostream>
#include <SDL.h>
#include "engine.h"
#include "../graphics/colors.h"
#include "../graphics/shape.h"
#include "../graphics/circle.h"
#include "../graphics/polygon.h"
#include "../graphics/polygonFactory.h"
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

        f1 = false;
        f3 = false;
        debugMode = false;
        previousF3State = true;

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
    // Testing rotation
    auto rect1 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT/2), 200, 100, BRIGHT_BLUE);
    auto rb1 = std::make_unique<RigidBody>(std::move(rect1), 40.0f, 1.0f, 0.0f);
    add_rigid_body(std::move(rb1));

    auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 + 120, WINDOW_HEIGHT/2), 200, 100, GOLDEN_YELLOW);
    auto rb2 = std::make_unique<RigidBody>(std::move(rect2), 40.0f, 1.0f, 0.0f);
    add_rigid_body(std::move(rb2));

    auto rect3 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT/2 + 130), 200, 100, WHITE);
    auto rb3 = std::make_unique<RigidBody>(std::move(rect3), 40.0f, 1.0f, 0.0f);
    add_rigid_body(std::move(rb3));


    // Testing positional correction
    // auto rect1 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 4 + 80, WINDOW_HEIGHT/4), 44, 44, LIGHT_BLUE);
    // auto rb1 = std::make_unique<RigidBody>(std::move(rect1), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb1));

    // auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT/2), 200, 100, GOLDEN_YELLOW);
    // auto rb2 = std::make_unique<RigidBody>(std::move(rect2), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb2));

    // auto staticRect = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 200), 50, 50, GREEN);
    // auto staticRb = std::make_unique<RigidBody>(std::move(staticRect), 0.0f);
    // add_rigid_body(std::move(staticRb));


    // Polygon vs Polygon
    // auto rect1 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT/2), 200, 100, BRIGHT_BLUE);
    // auto rb1 = std::make_unique<RigidBody>(std::move(rect1), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb1));

    // auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 + 120, WINDOW_HEIGHT/2), 200, 100, GOLDEN_YELLOW);
    // auto rb2 = std::make_unique<RigidBody>(std::move(rect2), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb2));

    // auto rect3 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 + 120, WINDOW_HEIGHT/4), 200, 100, GOLDEN_YELLOW);
    // auto rb3 = std::make_unique<RigidBody>(std::move(rect3), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb3));


    // Circle vs Circle
    // auto circ1 = std::make_unique<Circle>(Vector2(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT/2), 60, BRIGHT_BLUE);
    // auto rb1 = std::make_unique<RigidBody>(std::move(circ1), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb1));

    // auto circ2 = std::make_unique<Circle>(Vector2(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT/2), 60, GOLD_YELLOW);
    // auto rb2 = std::make_unique<RigidBody>(std::move(circ2), 40.0f, 1.0f, 0.0f);    
    // add_rigid_body(std::move(rb2));


    // Polygon vs Circle
    // auto rect1 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT/2), 200, 100, BRIGHT_BLUE);
    // auto rb1 = std::make_unique<RigidBody>(std::move(rect1), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb1));

    // auto circ2 = std::make_unique<Circle>(Vector2(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT/2), 60, GOLD_YELLOW);
    // auto rb2 = std::make_unique<RigidBody>(std::move(circ2), 40.0f, 1.0f, 0.0f);    
    // add_rigid_body(std::move(rb2));


    // Circle vs Polygon
    // auto circ1 = std::make_unique<Circle>(Vector2(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT/2), 60, BRIGHT_BLUE);
    // auto rb1 = std::make_unique<RigidBody>(std::move(circ1), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb1));

    // auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 2 + 120, WINDOW_HEIGHT/2), 200, 100, GOLDEN_YELLOW);
    // auto rb2 = std::make_unique<RigidBody>(std::move(rect2), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb2));


    // Polygon vs Circle vs Polygon
    // auto rect1 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 4, WINDOW_HEIGHT/2), 200, 100, GOLDEN_YELLOW);
    // auto rb1 = std::make_unique<RigidBody>(std::move(rect1), 40.0f, 1.0f, 0.0f);

    // auto circ1 = std::make_unique<Circle>(Vector2(WINDOW_WIDTH / 2 , WINDOW_HEIGHT/2), 60, BRIGHT_BLUE);
    // auto rb2 = std::make_unique<RigidBody>(std::move(circ1), 40.0f, 1.0f, 0.0f);    

    // auto rect2 = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT/2), 200, 100, GOLDEN_YELLOW);
    // auto rb3 = std::make_unique<RigidBody>(std::move(rect2), 40.0f, 1.0f, 0.0f);
    
    // auto circ2 = std::make_unique<Circle>(Vector2(WINDOW_WIDTH / 2 , WINDOW_HEIGHT/4), 60, BLUE_GREEN);
    // auto rb4 = std::make_unique<RigidBody>(std::move(circ2), 40.0f, 1.0f, 0.0f);    
    // add_rigid_body(std::move(rb1)); // Polígono
    // add_rigid_body(std::move(rb2)); // Círculo
    // add_rigid_body(std::move(rb3)); // Polígono
    // add_rigid_body(std::move(rb4)); // Círculo


    // Bodies coliding simulation
    // auto rectA = std::make_unique<Rectangle>(Vector2(300, 400), 44, 44, LIGHT_BLUE);
    // auto rb1 = std::make_unique<RigidBody>(std::move(rectA), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb1));

    // auto rectB = std::make_unique<Rectangle>(Vector2(400, 250), 44, 44, GOLDEN_YELLOW);
    // auto rb2 = std::make_unique<RigidBody>(std::move(rectB), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb2));

    // auto rectC = std::make_unique<Rectangle>(Vector2(180, 250), 44, 44, ACQUAMARINE);
    // auto rb3 = std::make_unique<RigidBody>(std::move(rectC), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb3));

    // auto circleA = std::make_unique<Circle>(Vector2(600, 300), 25, LEMON_YELLOW);
    // auto rb4 = std::make_unique<RigidBody>(std::move(circleA), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb4));

    // auto circleB = std::make_unique<Circle>(Vector2(600, 100), 25, SAFFRON_YELLOW);
    // auto rb5 = std::make_unique<RigidBody>(std::move(circleB), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb5));
 
    // auto staticRect = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 200), 50, 50, GREEN);
    // auto staticRb = std::make_unique<RigidBody>(std::move(staticRect), 0.0f);
    // add_rigid_body(std::move(staticRb));

    // auto pent = PolygonFactory::createPolygon(5, Vector2(200, 200), 60, WHITE);
    // auto rb6 =  std::make_unique<RigidBody>(std::move(pent), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb6));

    // auto tri = PolygonFactory::createPolygon(3, Vector2(400, 400), 60, WHITE);
    // auto rb7 =  std::make_unique<RigidBody>(std::move(tri), 40.0f, 1.0f, 0.0f);
    // add_rigid_body(std::move(rb7));
}

void Engine::setupWorldConstraints() {
    /* The following lines add the boundaries to the "world" */
    auto rectLeftWall = std::make_unique<Rectangle>(Vector2(0 - 6, WINDOW_HEIGHT/2), 10, WINDOW_HEIGHT, GREEN);
    auto rbLeftWall = std::make_unique<RigidBody>(std::move(rectLeftWall), 0.0f);
    add_rigid_body(std::move(rbLeftWall));

    auto rectRightWall = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH + 6, WINDOW_HEIGHT/2), 10, WINDOW_HEIGHT, GREEN);
    auto rbRightWall = std::make_unique<RigidBody>(std::move(rectRightWall), 0.0f);
    add_rigid_body(std::move(rbRightWall));

    auto rectFloor = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT + 5), WINDOW_WIDTH, 10, GREEN);
    auto rbFloor = std::make_unique<RigidBody>(std::move(rectFloor), 0.0f);
    add_rigid_body(std::move(rbFloor));

    auto rectCeiling = std::make_unique<Rectangle>(Vector2(WINDOW_WIDTH/2, 0 - 6), WINDOW_WIDTH, 10, GREEN);
    auto rbCeiling = std::make_unique<RigidBody>(std::move(rectCeiling), 0.0f);
    add_rigid_body(std::move(rbCeiling));
}


void Engine::setupScenario() {
    const std::string message = " ";
    auto text = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, 10), WHITE);
    add_text(std::move(text));
    
    auto text1 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(WINDOW_WIDTH/2, 10), WHITE);  
    add_text(std::move(text1));

    auto text2 = std::make_unique<Text>("../assets/fonts/Arial-Unicode.ttf", message, 24, Vector2(10, WINDOW_HEIGHT - 70), WHITE);
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

    // Text recovery
    // Text* text = texts[0].get();  // Upper right
    Text* text1 = texts[1].get();
    Text* text2 = texts[2].get(); // Bottow left

    text1->setDirectionRightToLeft();
    if (debugMode) {
        // text->setMessage("Debugging");
    } else {
        // text->setMessage(" "); // Not debugging
        // text1->setMessage(" ");
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
            rb->setAngularVelocity(0.0f);
            rb->setLinearVelocity(Vector2::Null);

            rb->getShape()->resetPosition();
            rb->getShape()->setOverlap(false);
        }        
    }


    RigidBody* rb1 = rigidBodies[0].get();    
    RigidBody* rb2 = rigidBodies[1].get();
    
    // Shape A/Player 1
    if (move_left)    { rb2->addForce(Vector2(-FORCE, 0)); }
    if (move_right)   { rb2->addForce(Vector2(FORCE, 0)); }
    if (move_up)      { rb2->addForce(Vector2(0, -FORCE)); } 
    if (move_down)    { rb2->addForce(Vector2(0, FORCE)); }
    if (rotate_left)  { rb2->addAngularVelocity(-ANGULAR_F); }
    if (rotate_right) { rb2->addAngularVelocity(ANGULAR_F); }

    // Shape B/Player 2
    if (move_leftB)    { rb1->addForce(Vector2(-FORCE, 0)); }
    if (move_rightB)   { rb1->addForce(Vector2(FORCE, 0)); }
    if (move_upB)      { rb1->addForce(Vector2(0, -FORCE)); }
    if (move_downB)    { rb1->addForce(Vector2(0, FORCE)); }
    if (rotate_leftB)  { rb1->addAngularVelocity(-ANGULAR_F); }
    if (rotate_rightB) { rb1->addAngularVelocity(ANGULAR_F); }

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
    }

     for (size_t i = 0; i < rigidBodies.size() - 1; i++) {
        for (size_t j = i + 1; j < rigidBodies.size(); j++) {
 
            bool flag = false;
            auto result = Collision::collisionDetection(*rigidBodies[i].get(), *rigidBodies[j].get());
            // auto result = Collision::checkCollision(*rigidBodies[i]->getShape(), *rigidBodies[j]->getShape());
            if (result) {
                flag = true;           
                
                if (!debugMode) {
                    // To be used when moving without physics
                    // Collision::simplePositionCorrection(*rigidBodies[i].get(), *rigidBodies[j].get(), *result.get());
    
                    // Resolving linear collisions(no angular movement)
                    Collision::positionCorrection(*rigidBodies[i].get(), *rigidBodies[j].get(), *result.get());
                    Collision::resolveLinearCollision(*rigidBodies[i].get(), *rigidBodies[j].get(), *result.get());
                    // Collision::resolveCollision(*rigidBodies[i].get(), *rigidBodies[j].get(), *result.get());
                }

                if (debugMode) {
                    std::string str = result->toString();
                    text1->setMessage(str);
                }

                manifolds.push_back(std::move(result));
            }

            if (debugMode) {
                rigidBodies[i]->getShape()->setOverlap(flag | rigidBodies[i]->getShape()->getOverlap());
                rigidBodies[j]->getShape()->setOverlap(flag | rigidBodies[j]->getShape()->getOverlap());
            }
        }
    }


    last_frame_time = SDL_GetTicks();
}

void Engine::rendering() {
	graphics->clearScreen(BLACK.r, BLACK.g, BLACK.b, 255);  // Clear screen using the Graphics object

    for (const auto& shape : shapes) {
        shape->draw(graphics->getRenderer());  // Use Graphics renderer to draw shapes
    }

    if (debugMode) {
        for (const auto& manifold : manifolds) {
            manifold->draw(graphics->getRenderer());  // Draw the manifold (collision details)
        } 
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
