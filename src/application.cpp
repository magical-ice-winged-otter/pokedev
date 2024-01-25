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
static std::vector<ImGuiWindow*> s_tools {};

static void reloadConfig() {
    s_config.readData(
        CUSTOM_NAME("gameSettings", Application::settings)
    );
}

static void saveConfig() {
    s_config.writeData(
        CUSTOM_NAME("gameSettings", Application::settings)
    );
}

void Application::init() {
    reloadConfig();
    loaders.init(settings.projectPath);

    s_tools.push_back(new ImGuiDemoTool{});
    s_tools.push_back(new ShortcutsTool{});
    s_tools.push_back(new PorytilesTool{Platform::getRenderer()});
}

void Application::shutdown() {
    for (int i = 0; i < s_tools.size(); ++i) {
        delete s_tools[i];
    }

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
            for (int i = 0; i < s_tools.size(); ++i) {
                ImGui::MenuItem(s_tools[i]->getName(), nullptr, &s_tools[i]->isActive);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    ImGui::Begin("settings");
    settings.draw();
    ImGui::End();

    // render all tools
    for (int i = 0; i < s_tools.size(); ++i) {
        s_tools[i]->render();
    }
}
