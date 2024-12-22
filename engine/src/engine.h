#ifndef _engine_h
#define _engine_h

#include <SDL.h>
#include <memory>
#include "../../graphics/graphics.h"
#include "../../graphics/shape.h"
#include "../../collision/manifold.h"
#include "../../input_manager/input_manager.h"

class Engine {
private:
	bool engine_is_running;
	unsigned int last_frame_time;

	// Arrow control (it may be figure A or player 1)
	bool move_left, move_right, move_up, move_down, rotate_left, rotate_right;

	// Arrow control (it may be figure B or player 2)
	bool move_leftB, move_rightB, move_upB, move_downB, rotate_leftB, rotate_rightB;

	// Special Keys
	// f1 -> Reset the position of the objects
	// f2 -> change the collision detection for polygons
	// f3 -> debugger on/off
	bool f1, f2, f3;
	bool debugMode;


    std::unique_ptr<Graphics> graphics;  // Use unique_ptr to manage the lifetime of the Graphics object
    std::unique_ptr<InputManager> input_manager;

	std::vector<std::unique_ptr<Shape>> shapes;
	std::vector<std::unique_ptr<Manifold>> manifolds;

public:
	Engine();
	~Engine();	

	bool initialization();

	void input_processing();
	void updating();
	void rendering();
	
	bool is_game_running();
    void add_shape(std::unique_ptr<Shape> shape); // Accept unique_ptr
    std::vector<std::unique_ptr<Shape>>& get_shapes() {
        return shapes;  // Return reference to the internal vector
    }
};

#endif
