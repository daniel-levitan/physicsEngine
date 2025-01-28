#ifndef _constants_h
#define _constants_h

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SPEED 150

#define MOVEMENT_SPEED 150
#define ROTATION_SPEED 5

#define FORCE 5000

#define DAMPING 0.99f
// #define DAMPING 1

enum class CollisionMode {
	PIXEL,
	SAT,
	DIAGONAL,
	DIAGONAL_RES,
	SAT_RES,
	TEST,
    COUNT  
};

// constexpr float DEFAULT_GRAVITY = 9.81f;  // m/s^2
constexpr float DEFAULT_GRAVITY = 5000;  // Instead of using the acceleration, we use a force


#endif