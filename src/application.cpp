#include "application.hpp"
#include "platform.hpp"
#include "porytiles/porytiles_gui.hpp"
#include "serializer.hpp"
#include "trainers/mons/mon_editor.hpp"
#include "game_loaders.hpp"
#include "shortcuts/shortcuts.hpp"
#include "pokedev_tool.hpp"
#include "tools/imgui_demo_tool.hpp"

#define ARR_LEN(array) (sizeof(array) / sizeof(array[0]))

static struct WindowState {
    bool showSettings{};
    bool showImGuiDemo{};
    bool showPrimaryCompiler{};
    bool showPrimaryDecompiler{};
    bool showSecondaryCompiler{};
    bool showSecondaryDecompiler{};
    bool showShortcuts{true};

    template <class Archive>
    void serialize(Archive& archive) {
        archive(
            CUSTOM_NAME("showSettings", showSettings),
            CUSTOM_NAME("showImGuiDemo", showImGuiDemo),
            CUSTOM_NAME("showPrimaryCompilerTool", showPrimaryCompiler),
            CUSTOM_NAME("showSecondaryCompilerTool", showSecondaryCompiler),
            CUSTOM_NAME("showPrimaryDecompilerTool", showPrimaryDecompiler),
            CUSTOM_NAME("showSecondaryDecompilerTool", showSecondaryDecompiler),
            CUSTOM_NAME("showShortcuts", showShortcuts)
        );
    }
} window;

static ConfigFile s_config{"pokedev_config.json"};
static PorytilesGui s_porytilesGui{};
static ShortcutGui s_shortcutGui {};

GameLoaders Application::loaders {};
GameSettings Application::settings {};

static void reloadConfig() {
    s_config.readData(
        CUSTOM_NAME("porytilesGui", s_porytilesGui),
        CUSTOM_NAME("shortcutGui", s_shortcutGui),
        CUSTOM_NAME("windowState", window),
        CUSTOM_NAME("gameSettings", Application::settings)
    );
}

static void saveConfig() {
    s_config.writeData(
        CUSTOM_NAME("porytilesGui", s_porytilesGui),
        CUSTOM_NAME("shortcutGui", s_shortcutGui),
        CUSTOM_NAME("windowState", window),
        CUSTOM_NAME("gameSettings", Application::settings)
    );
}

static PokeDevTool* s_tools[] {
    new ImGuiDemoTool {},
}; 

void Application::init() {
    //    loaders = createLoaders(settings.projectPath); // todo: slow
    s_porytilesGui.init(Platform::getRenderer());
    s_shortcutGui.init();
}

void Application::shutdown() {
    s_porytilesGui.shutdown();
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
            ImGui::MenuItem("Settings", nullptr, &window.showSettings);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
            for (int i = 0; i < ARR_LEN(s_tools); ++i) {
                ImGui::MenuItem(s_tools[i]->name, nullptr, &s_tools[i]->isActive);
            }
            ImGui::MenuItem("Primary Compiler", nullptr, &window.showPrimaryCompiler);
            ImGui::MenuItem("Primary Decompiler", nullptr, &window.showPrimaryDecompiler);
            ImGui::MenuItem("Secondary Compiler", nullptr, &window.showSecondaryCompiler);
            ImGui::MenuItem("Secondary Decompiler", nullptr, &window.showSecondaryDecompiler);
            ImGui::MenuItem("Shortcuts", nullptr, &window.showShortcuts);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();

    for (int i = 0; i < ARR_LEN(s_tools); ++i) {
        if (s_tools[i]->isActive) {
            s_tools[i]->renderWindow());
        }
    }

    if (window.showPrimaryCompiler) {
        s_porytilesGui.drawPrimaryCompilerWindow(&window.showPrimaryCompiler);
    }
    if (window.showPrimaryDecompiler) {
        s_porytilesGui.drawPrimaryDecompilerWindow(&window.showPrimaryDecompiler);
    }
    if (window.showSecondaryCompiler) {
        s_porytilesGui.drawSecondaryCompilerWindow(&window.showSecondaryCompiler);
    }
    if (window.showSecondaryDecompiler) {
        s_porytilesGui.drawSecondaryDecompilerWindow(&window.showSecondaryDecompiler);
    }
    if (window.showShortcuts) {
        s_shortcutGui.draw(window.showShortcuts);
    }
    if (window.showSettings && ImGui::Begin("Settings", &window.showSettings)) {
        settings.draw();
        if (ImGui::CollapsingHeader("Porytiles")) {
            s_porytilesGui.drawSettings();
        }
        ImGui::End();
    }
}
