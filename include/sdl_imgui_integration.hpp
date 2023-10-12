#ifndef PORYTILES_GUI_SDL_IMGUI_WRAPPER_HPP
#define PORYTILES_GUI_SDL_IMGUI_WRAPPER_HPP

#include <SDL.h>

namespace SDLImGuiIntegration
{
    void init(SDL_Window* window, SDL_Renderer* renderer);
    void shutdown();
    void processEvent(SDL_Event* event);
    void preRender();
    void postRender();
}

#endif // PORYTILES_GUI_SDL_IMGUI_WRAPPER_HPP