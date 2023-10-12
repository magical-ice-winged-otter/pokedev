#include "application.hpp"
#include "platform.hpp"

static struct WindowState
{
    bool showSettings{};
    bool showImGuiDemo{};
    bool showPrimaryCompilerTool{};
    bool showPrimaryDecompilerTool{};
    bool showSecondaryCompilerTool{};
    bool showSecondaryDecompilerTool{};

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CUSTOM_NAME("showSettings", showSettings),
            CUSTOM_NAME("showImGuiDemo", showImGuiDemo),
            CUSTOM_NAME("showPrimaryCompilerTool", showPrimaryCompilerTool),
            CUSTOM_NAME("showSecondaryCompilerTool", showSecondaryCompilerTool),
            CUSTOM_NAME("showPrimaryDecompilerTool", showPrimaryDecompilerTool),
            CUSTOM_NAME("showSecondaryDecompilerTool", showSecondaryDecompilerTool)
        );
    }
} ws;

static ConfigFile s_config{"pokedev_config.json"};
static PorytilesGui s_porytilesGui{};

static void reloadConfig()
{
    s_config.readData(
        CUSTOM_NAME("porytilesGui", s_porytilesGui),
        CUSTOM_NAME("windowState", ws)
    );
}

static void saveConfig()
{
    s_config.writeData(
        CUSTOM_NAME("porytilesGui", s_porytilesGui),
        CUSTOM_NAME("windowState", ws)
    );
}

void Application::init()
{
    reloadConfig();
    s_porytilesGui.init(Platform::getRenderer());
}

void Application::shutdown()
{
    s_porytilesGui.shutdown();
    saveConfig();
}

void Application::render()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    int flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Main Window", nullptr, flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Reload Config"))
                reloadConfig();

            if (ImGui::MenuItem("Save Config"))
                saveConfig();

            ImGui::MenuItem("Settings", nullptr, &ws.showSettings);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Primary Compiler", nullptr, &ws.showPrimaryCompilerTool);
            ImGui::MenuItem("Primary Decompiler", nullptr, &ws.showPrimaryDecompilerTool);
            ImGui::MenuItem("Secondary Compiler", nullptr, &ws.showSecondaryCompilerTool);
            ImGui::MenuItem("Secondary Decompiler", nullptr, &ws.showSecondaryDecompilerTool);
            ImGui::MenuItem("ImGui Demo", nullptr, &ws.showImGuiDemo);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    if (ws.showPrimaryCompilerTool) s_porytilesGui.renderPrimaryCompilerWindow(&ws.showPrimaryCompilerTool);
    if (ws.showPrimaryDecompilerTool) s_porytilesGui.renderPrimaryDecompilerWindow(&ws.showPrimaryDecompilerTool);
    if (ws.showSecondaryCompilerTool) s_porytilesGui.renderSecondaryCompilerWindow(&ws.showSecondaryCompilerTool);
    if (ws.showSecondaryDecompilerTool) s_porytilesGui.renderSecondaryDecompilerWindow(&ws.showSecondaryDecompilerTool);
    if (ws.showImGuiDemo) ImGui::ShowDemoWindow(&ws.showImGuiDemo);

    if (ws.showSettings && ImGui::Begin("Settings", &ws.showSettings))
    {
        s_porytilesGui.renderSettings();
        ImGui::End();
    }
}
