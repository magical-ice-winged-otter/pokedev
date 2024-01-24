#include "application.hpp"
#include "platform.hpp"
#include "serializer.hpp"
#include "trainers/mons/mon_editor.hpp"
#include "game_loaders.hpp"
#include "tools/shortcuts_tool.hpp"
#include "tools/imgui_demo_tool.hpp"
#include "tools/porytiles_tool.hpp"

static ConfigFile s_config{"pokedev_config.json"};

GameLoaders Application::loaders {};
GameSettings Application::settings {};

static void reloadConfig() {
    s_config.readData(
        // CUSTOM_NAME("shortcutGui", s_shortcutGui),
        CUSTOM_NAME("gameSettings", Application::settings)
    );
}

static void saveConfig() {
    s_config.writeData(
        // CUSTOM_NAME("shortcutGui", s_shortcutGui),
        CUSTOM_NAME("gameSettings", Application::settings)
    );
}

static size_t s_toolCount;
static PokeDevTool** s_tools;

void Application::init() {
    s_tools = new PokeDevTool*[] {
        new ImGuiDemoTool {},
        new ShortcutsTool {},
        new PorytilesTool {Platform::getRenderer()},
    };
    s_toolCount = 3;
    //    loaders = createLoaders(settings.projectPath); // todo: slow
}

void Application::shutdown() {
    saveConfig();
}

void Application::render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    int flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_MenuBar;

    if (ImGui::IsKeyPressed(ImGuiKey_F11, false)) {
        // toggle fullscreen
        SDL_Window* sdlWindow = Platform::getWindow();
        bool isFullscreen = (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
        SDL_SetWindowFullscreen(sdlWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    ImGui::Begin("Main Window", nullptr, flags);

    if (ImGui::BeginMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Reload Config")) {
                reloadConfig();
            }
            if (ImGui::MenuItem("Save Config")) {
                saveConfig();
            }
            ImGui::EndMenu();
        }

        // show tools in menu bar
        if (ImGui::BeginMenu("Tools")) {
            for (int i = 0; i < s_toolCount; ++i) {
                ImGui::MenuItem(s_tools[i]->name, nullptr, &s_tools[i]->isActive);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    // render all tools
    for (int i = 0; i < s_toolCount; ++i) {
        if (s_tools[i]->isActive && ImGui::Begin(s_tools[i]->name, &s_tools[i]->isActive)) {
            s_tools[i]->renderWindow();
        }
    }
}
