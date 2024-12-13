#ifndef _engine_h
#define _engine_h

#include <SDL.h>
#include <memory>
#include "../graphics/graphics.h"
#include "../graphics/shape.h"


class Engine {
private:
	bool engine_is_running;
	unsigned int last_frame_time;

    std::unique_ptr<Graphics> graphics;  // Use unique_ptr to manage the lifetime of the Graphics object

	std::vector<std::unique_ptr<Shape>> shapes;

public:
	Engine();
	~Engine();	

	bool initialization();

	void input_processing();
	void updating();
	void rendering();
	
	bool is_game_running();
    void add_shape(std::unique_ptr<Shape> shape); // Accept unique_ptr
};

#endif
