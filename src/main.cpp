#include <cstdio>
#include <imgui_impl_sdl2.h>
#include <SDL.h>
#include <filesystem>
#include "serializer.hpp"
#include "sdl_imgui_integration.hpp"
#include "porytiles_gui.hpp"
#include "platform.hpp"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error starting SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Porytiles GUI",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        return -1;
    }

    Platform::init();
    SDLImGuiIntegration::init(window, renderer);
    Serializer::init(std::filesystem::path{argv[0]}.remove_filename() / "porytiles_gui_config.json");
    PorytilesGui::init(renderer);

    bool done = false;

    while (!done)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            SDLImGuiIntegration::processEvent(&event);

            if (event.type == SDL_QUIT)
                done = true;
        }

        SDL_RenderClear(renderer);
        SDLImGuiIntegration::preRender();
        PorytilesGui::render();        
        SDLImGuiIntegration::postRender();
        SDL_RenderPresent(renderer);
    }

    Platform::shutdown();
    SDLImGuiIntegration::shutdown();
    PorytilesGui::shutdown();
    Serializer::shutdown();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}