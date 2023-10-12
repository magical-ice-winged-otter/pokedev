#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "platform.hpp"
#include "platform/platform_windows.hpp"

static SDL_Window* s_window {};
static SDL_Renderer* s_renderer {};
static bool s_wantsToQuit {false};

void Platform::init()
{
    // Initialize SDL and our window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error starting SDL: %s\n", SDL_GetError());
        return;
    }

    s_window = SDL_CreateWindow(
            "Porytiles GUI",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280,
            720,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (s_renderer == nullptr)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        return;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(s_window, s_renderer);
    ImGui_ImplSDLRenderer2_Init(s_renderer);

    // Initialize platform-dependent code
#ifdef _WIN32
    Platform::Windows::init();
#endif
}

void Platform::shutdown()
{
    // Shutdown platform-dependent code
#ifdef _WIN32
    Platform::Windows::shutdown();
#endif

    // Shutdown ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Shutdown SDL
    SDL_DestroyRenderer(s_renderer);
    SDL_DestroyWindow(s_window);
    SDL_Quit();

    s_renderer = nullptr;
    s_window = nullptr;
}

void Platform::startFrame()
{
    // Flush the event loop
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            s_wantsToQuit = true;
    }

    // Reset the renderer for the next frame.
    SDL_RenderClear(s_renderer);
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Platform::endFrame()
{
    // Submit render data to be drawn.
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(s_renderer);
}

SDL_Renderer* Platform::getRenderer()
{
    return s_renderer;
}

SDL_Window* Platform::getWindow()
{
    return s_window;
}

bool Platform::wantsToQuit()
{
    return s_wantsToQuit;
}

bool Platform::tryPickFile(std::filesystem::path &outPath, const Platform::FilePickerOptions &options)
{
#ifdef _WIN32
    return Platform::Windows::tryPickFile(outPath, options);
#else
    printf("Platform does not support picking files!\n");
    return false;
#endif
}

bool Platform::tryPickFolder(std::filesystem::path &outPath, const Platform::FilePickerOptions &options)
{
#ifdef _WIN32
    return Platform::Windows::tryPickFolder(outPath, options);
#else
    printf("Platform does not support picking folders!\n");
    return false;
#endif
}

void Platform::openPath(const std::filesystem::path &path)
{
#ifdef _WIN32
    return Platform::Windows::openPath(path);
#else
    printf("Platform does not support opening paths!\n");
    return false;
#endif
}
