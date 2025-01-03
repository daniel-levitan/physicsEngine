#ifndef _constants_h
#define _constants_h

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SPEED 150

#define MOVEMENT_SPEED 150
#define ROTATION_SPEED 5

#define NUM_COLLISION_MODES 5

enum class CollisionMode {
	PIXEL,
	SAT,
	DIAGONAL,
	DIAGONAL_RES,
	SAT_RES,
    COUNT  
};

#endif