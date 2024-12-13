#ifndef _engine_h
#define _engine_h

#include <SDL.h>
#include <memory>
#include "../graphics/shape.h"


// 2. A inicialização e o cleanup podem ser feitos com o construtor e destrutor?
// ✓ 3. Como testar se a inicialização deu certo? Uso outra variável/flag?
// 4. Crio classes específicas para atender a essa recomendação: Use constructors and 
//    destructors for objects like SDL_Window, SDL_Renderer, and shapes to ensure proper 
//    lifecycle management.
//    E essa? Decouple SDL and Engine Logic: Abstract SDL-specific code (e.g., 
//    SDL_CreateWindow, SDL_RenderClear) behind an interface or class so the engine is not
//    tightly coupled to SDL.
// 5. A função de inicialização deve receber os parâmetros de tamanho da janela?

class Engine {
private:
	bool engine_is_running;
	unsigned int last_frame_time;

	// SDL_Window* window;
	// SDL_Renderer* renderer;
	// Using unique_ptr with a custom deleter to manage SDL resources
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;
	
    std::vector<Shape*> shapes;  // Vector to hold shapes (polymorphic)


public:
	Engine();
	

	bool initialization();
	void cleanup();

	void input_processing();
	void updating();
	void rendering();
	
	bool is_game_running();
    void add_shape(Shape* shape); 

	~Engine();
};

#endif
