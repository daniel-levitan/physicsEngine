#include "input_manager.h"

void InputManager::processInput(bool& isRunning) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            keyStates[event.key.keysym.sym] = true;  // Set key state to 'down'
            break;

        case SDL_KEYUP:
            keyStates[event.key.keysym.sym] = false; // Set key state to 'up'
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseButtonStates[event.button.button] = true;  // Set mouse button state to 'pressed'
            break;

        case SDL_MOUSEBUTTONUP:
            mouseButtonStates[event.button.button] = false; // Set mouse button state to 'released'
            break;        
		}
	}
}

bool InputManager::isKeyDown(SDL_Keycode key) {
    return keyStates[key];  // Return true if the key is pressed
}

bool InputManager::isKeyUp(SDL_Keycode key) {
    return !keyStates[key];  // Return true if the key is released
}

bool InputManager::isMouseButtonPressed(Uint8 button) {
    return mouseButtonStates[button];  // Return true if the mouse button is pressed
}

bool InputManager::isMouseButtonReleased(Uint8 button) {
    return !mouseButtonStates[button];  // Return true if the mouse button is released
}

void InputManager::getMousePosition(int& x, int& y) {
    SDL_GetMouseState(&x, &y);  // Get the mouse position relative to the window
}