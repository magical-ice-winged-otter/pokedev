#include <cstdio>
#include <imgui.h>
#include "file_dialog.hpp"
#include "porytiles_gui.hpp"
#include "serializer.hpp"

using namespace std;

namespace PorytilesGui
{
    static filesystem::path s_projectPath {};
    static filesystem::path s_behaviorsHeaderPath {};
    static int s_currentMode {};
    static bool s_showPrimaryCompilerTool {};
    static bool s_showPrimaryDecompilerTool {};
    static bool s_showSecondaryDecompilerTool {};
    static bool s_showSecondaryCompilerTool {};

    void init()
    {
        s_projectPath = Serializer::load("projectPath", filesystem::path{});
        s_behaviorsHeaderPath = Serializer::load("behaviorsHeaderPath", filesystem::path{});
        s_currentMode = Serializer::load("currentMode", 0);
        s_showPrimaryCompilerTool = Serializer::load("showPrimaryCompiler", false);
        s_showPrimaryDecompilerTool = Serializer::load("showPrimaryDecompiler", false);
        s_showSecondaryCompilerTool = Serializer::load("showSecondaryCompiler", false);
        s_showSecondaryDecompilerTool = Serializer::load("showSecondaryDecompiler", false);
    }

    void shutdown()
    {
        Serializer::store("projectPath", s_projectPath);
        Serializer::store("behaviorsHeaderPath", s_behaviorsHeaderPath);
        Serializer::store("currentMode", s_currentMode);
        Serializer::store("showPrimaryCompiler", s_showPrimaryCompilerTool);
        Serializer::store("showPrimaryDecompiler", s_showPrimaryDecompilerTool);
        Serializer::store("showSecondaryCompiler", s_showSecondaryCompilerTool);
        Serializer::store("showSecondaryDecompiler", s_showSecondaryDecompilerTool);
    }

    void render()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Primary Compiler", nullptr, &s_showPrimaryCompilerTool);
                ImGui::MenuItem("Primary Decompiler", nullptr, &s_showPrimaryDecompilerTool);
                ImGui::MenuItem("Secondary Compiler", nullptr, &s_showSecondaryCompilerTool);
                ImGui::MenuItem("Secondary Decompiler", nullptr, &s_showSecondaryDecompilerTool);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::ShowDemoWindow();

        if (s_showPrimaryCompilerTool && ImGui::Begin("Primary Compiler", &s_showPrimaryCompilerTool))
        {
            if (ImGui::Button("Edit"))
                FileDialog::tryPickFolder(s_projectPath);

            ImGui::SameLine();
            ImGui::Text("Source Primary Path: %s", s_projectPath.string().c_str());
            ImGui::End();
        }
        if (s_showPrimaryDecompilerTool && ImGui::Begin("Primary Decompiler", &s_showPrimaryDecompilerTool))
        {
            ImGui::End();
        }
        if (s_showSecondaryCompilerTool && ImGui::Begin("Secondary Compiler", &s_showSecondaryCompilerTool))
        {
            ImGui::End();
        }
        if (s_showSecondaryDecompilerTool && ImGui::Begin("Secondary Decompiler", &s_showSecondaryDecompilerTool))
        {
            ImGui::End();
        }

        ImGui::SeparatorText("General Settings");

        if (ImGui::Button("Edit"))
            FileDialog::tryPickFolder(s_projectPath);

        ImGui::SameLine();
        ImGui::Text("Project path: %s", s_projectPath.string().c_str());

        if (ImGui::Button("Edit"))
            FileDialog::tryPickFile(s_behaviorsHeaderPath, "h,hpp");

        ImGui::SameLine();
        ImGui::Text("Behaviors Header: %s", s_behaviorsHeaderPath.string().c_str());

        ImGui::End();
    }
}
