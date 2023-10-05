#include <SDL.h>

namespace SDLImGui
{
    void init(SDL_Window* window, SDL_Renderer* renderer);
    void shutdown();
    void processEvent(SDL_Event* event);
    void preRender();
    void postRender();
}