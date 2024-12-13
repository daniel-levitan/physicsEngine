#ifndef _input_manager_h
#define _input_manager_h

#include <SDL.h>
#include <unordered_map>

class InputManager {
private: 
	std::unordered_map<SDL_Keycode, bool> keyStates;
	std::unordered_map<Uint8, bool> mouseButtonStates;

public:
	InputManager() = default;
	~InputManager() = default;

	void processInput(bool& isRunning);

	bool isKeyDown(SDL_Keycode key);
	bool isKeyUp(SDL_Keycode key);

	bool isMouseButtonPressed(Uint8 button);
	bool isMouseButtonReleased(Uint8 button);
	void getMousePosition(int& x, int& y);

};

#endif