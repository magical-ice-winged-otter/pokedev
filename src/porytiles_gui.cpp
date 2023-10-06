#include <imgui.h>
#include "file_dialog.hpp"
#include "porytiles_gui.hpp"
#include "serializer.hpp"

using namespace std;

// todo: tooltips for everything would be nice...
// todo: a way to reset config / save config / load config would be nice...
// todo: move all the state out of static global scope and into structure? would be easier to pass around

namespace PorytilesGui
{
    // General Settings
    static filesystem::path s_projectPath {};
    static filesystem::path s_behaviorsHeaderPath {};

    static float s_transparency[3] {1, 0, 1};
    static string s_defaultBehavior {"MB_NORMAL"};
    static bool s_useDualLayer {false};
    static string s_baseGame {"pokeemerald"};

    static int s_assignExploreCutoff {2};
    static string s_assignAlgorithm {"dfs"};
    static string s_bestBranches {"smart"};

    static int s_primaryAssignExploreCutoff {2};
    static string s_primaryAssignAlgorithm {"dfs"};
    static string s_primaryBestBranches {"smart"};

    // Tools
    static bool s_showPrimaryCompilerTool {};
    static bool s_showPrimaryDecompilerTool {};
    static bool s_showSecondaryDecompilerTool {};
    static bool s_showSecondaryCompilerTool {};

    static filesystem::path s_primaryCompileOutputPath {};
    static filesystem::path s_primaryDecompileOutputPath {};
    static filesystem::path s_secondaryCompileOutputPath {};
    static filesystem::path s_secondaryDecompileOutputPath {};
    static filesystem::path s_sourcePrimaryPath {};
    static filesystem::path s_sourceSecondaryPath {};
    static filesystem::path s_sourcePartnerPrimaryPath {};
    static filesystem::path s_compiledPrimaryPath {};
    static filesystem::path s_compiledSecondaryPath {};
    static filesystem::path s_compiledPartnerPrimaryPath {};

    // GUI Settings
    static ImVec4 s_subTextColor {0.6, 0.6, 0.6, 1};
    static ImVec4 s_errorTextColor {1.0, 0.3, 0.3, 1};
    static int s_defaultBehaviorBufferSize {100};
    static int s_bestBranchesBufferSize {100};
    static int s_primaryBestBranchesBufferSize {100};

    void init()
    {
        s_defaultBehavior.resize(s_defaultBehaviorBufferSize);
        s_bestBranches.resize(s_bestBranchesBufferSize);
        s_primaryBestBranches.resize(s_primaryBestBranchesBufferSize);

        Serializer::registerValue("assignExploreCutoff", s_assignExploreCutoff);
        Serializer::registerValue("assignAlgorithm", s_assignAlgorithm);
        Serializer::registerValue("bestBranches", s_bestBranches);
        Serializer::registerValue("primaryAssignExploreCutoff", s_primaryAssignExploreCutoff);
        Serializer::registerValue("primaryAssignAlgorithm", s_primaryAssignAlgorithm);
        Serializer::registerValue("primaryBestBranches", s_primaryBestBranches);

        Serializer::registerValue("transparencyColor_R", s_transparency[0]);
        Serializer::registerValue("transparencyColor_G", s_transparency[1]);
        Serializer::registerValue("transparencyColor_B", s_transparency[2]);
        Serializer::registerValue("defaultBehavior", s_defaultBehavior);
        Serializer::registerValue("useDualLayer", s_useDualLayer);
        Serializer::registerValue("baseGame", s_baseGame);

        Serializer::registerValue("projectPath", s_projectPath);
        Serializer::registerValue("behaviorsHeaderPath", s_behaviorsHeaderPath);

        Serializer::registerValue("showPrimaryCompiler", s_showPrimaryCompilerTool);
        Serializer::registerValue("showPrimaryDecompiler", s_showPrimaryDecompilerTool);
        Serializer::registerValue("showSecondaryCompiler", s_showSecondaryCompilerTool);
        Serializer::registerValue("showSecondaryDecompiler", s_showSecondaryDecompilerTool);

        Serializer::registerValue("primaryCompileOutputPath", s_primaryCompileOutputPath);
        Serializer::registerValue("primaryDecompileOutputPath", s_primaryDecompileOutputPath);
        Serializer::registerValue("secondaryCompileOutputPath", s_secondaryCompileOutputPath);
        Serializer::registerValue("secondaryDecompileOutputPath", s_secondaryDecompileOutputPath);
    }

    void shutdown()
    {
        // No shutdown logic needed for now.
    }

    // todo: move this into custom ImGui utils file?
    static void ImGuiFolderPicker(const char* label, filesystem::path& path)
    {
        if (ImGui::Button("Edit"))
            FileDialog::tryPickFolder(path);

        ImGui::SameLine();
        ImGui::Text("%s", label);

        if (path.string().empty())
            ImGui::TextColored(s_errorTextColor, "No folder selected");

        else ImGui::TextColored(s_subTextColor, "%s", path.string().c_str());

        ImGui::Spacing(); ImGui::Spacing();
    }

    // todo: move this into custom ImGui utils file?
    static void ImGuiFilePicker(const char* label, filesystem::path& path, const char* filter = nullptr)
    {
        if (ImGui::Button("Edit"))
            FileDialog::tryPickFile(path, filter);

        ImGui::SameLine();
        ImGui::Text("%s", label);

        if (path.string().empty())
            ImGui::TextColored(s_errorTextColor, "No file selected");

        else ImGui::TextColored(s_subTextColor, "%s", path.string().c_str());

        ImGui::Spacing(); ImGui::Spacing();
    }

    void render()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);

        // Menu Bar
        {
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
        }

        // Windows
        {
            ImGui::ShowDemoWindow();
            if (s_showPrimaryCompilerTool && ImGui::Begin("Primary Compiler", &s_showPrimaryCompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    // todo: generate the compile command, copy to clipboard.
                }

                ImGuiFolderPicker("Output Path", s_primaryCompileOutputPath);
                ImGuiFolderPicker("Source Primary Path", s_sourcePrimaryPath);
                ImGui::SetItemTooltip("Path to a directory containing the source data for a primary set.");
                ImGui::End();
            }
            if (s_showPrimaryDecompilerTool && ImGui::Begin("Primary Decompiler", &s_showPrimaryDecompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    // todo: generate the compile command, copy to clipboard.
                }

                ImGuiFolderPicker("Output Path", s_primaryDecompileOutputPath);
                ImGuiFolderPicker("Source Secondary Path", s_sourceSecondaryPath);
                ImGuiFolderPicker("Source Partner Primary Path", s_sourceSecondaryPath);
                ImGui::End();
            }
            if (s_showSecondaryCompilerTool && ImGui::Begin("Secondary Compiler", &s_showSecondaryCompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    // todo: generate the compile command, copy to clipboard.
                }

                ImGuiFolderPicker("Output Path", s_secondaryCompileOutputPath);
                ImGuiFolderPicker("Compiled Primary Path", s_compiledPrimaryPath);

                ImGui::SeparatorText("Paired Primary Color Assignment Config");
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("Explore Cutoff", &s_primaryAssignExploreCutoff);

                if (ImGui::RadioButton("Depth-First Search", s_primaryAssignAlgorithm == "dfs")) s_primaryAssignAlgorithm = "dfs"; ImGui::SameLine();
                if (ImGui::RadioButton("Breadth-First Search", s_primaryAssignAlgorithm == "bfs")) s_primaryAssignAlgorithm = "bfs";

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Best Branches", s_primaryBestBranches.data(), s_primaryBestBranchesBufferSize);

                ImGui::End();
            }
            if (s_showSecondaryDecompilerTool && ImGui::Begin("Secondary Decompiler", &s_showSecondaryDecompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    // todo: generate the compile command, copy to clipboard.
                }

                ImGuiFolderPicker("Output Path", s_secondaryDecompileOutputPath);
                ImGuiFolderPicker("Compiled Secondary Path", s_compiledSecondaryPath);
                ImGuiFolderPicker("Compiled Partner Primary Path", s_compiledPartnerPrimaryPath);
                ImGui::End();
            }
        }

        // Tools
        {
            ImGui::SeparatorText("Tools");

            ImGui::Text("Primary"); ImGui::SameLine();
            s_showPrimaryCompilerTool |= ImGui::Button("Compiler##Primary"); ImGui::SameLine();
            s_showPrimaryDecompilerTool |= ImGui::Button("Decompiler##Primary");

            ImGui::Text("Secondary"); ImGui::SameLine();
            s_showSecondaryCompilerTool |= ImGui::Button("Compiler##Secondary"); ImGui::SameLine();
            s_showSecondaryDecompilerTool |= ImGui::Button("Decompiler##Secondary");
        }

        // Settings
        {
            ImGui::SeparatorText("Settings");

            if (ImGui::CollapsingHeader("Driver"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                ImGuiFolderPicker("Project Path", s_projectPath);
                ImGuiFilePicker("Behaviors Header File", s_behaviorsHeaderPath, "h,hpp");

                ImGui::Text("Palette Mode");
                static string s_paletteMode {"greyscale"};
                if (ImGui::RadioButton("True Color", s_paletteMode == "true-color")) s_paletteMode = "true-color"; ImGui::SameLine();
                if (ImGui::RadioButton("Greyscale", s_paletteMode == "greyscale")) s_paletteMode = "greyscale";

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
            if (ImGui::CollapsingHeader("Tileset (De)compilation"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                if (ImGui::RadioButton("Emerald", s_baseGame == "pokeemerald")) s_baseGame = "pokeemerald"; ImGui::SameLine();
                if (ImGui::RadioButton("Fire Red", s_baseGame == "pokefirered")) s_baseGame = "pokefirered"; ImGui::SameLine();
                if (ImGui::RadioButton("Ruby", s_baseGame == "pokeruby")) s_baseGame = "pokeruby";

                if (ImGui::RadioButton("Dual", s_useDualLayer)) s_useDualLayer = true; ImGui::SameLine();
                if (ImGui::RadioButton("Triple", !s_useDualLayer)) s_useDualLayer = false;

                ImGui::SetNextItemWidth(300);
                ImGui::ColorEdit3("Transparency Color", s_transparency);

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Default Behavior", s_defaultBehavior.data(), s_defaultBehaviorBufferSize);

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
            if (ImGui::CollapsingHeader("Color assignment Config"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("Assign Explore Cutoff", &s_assignExploreCutoff);

                if (ImGui::RadioButton("Depth-First Search", s_assignAlgorithm == "dfs")) s_assignAlgorithm = "dfs"; ImGui::SameLine();
                if (ImGui::RadioButton("Breadth-First Search", s_assignAlgorithm == "bfs")) s_assignAlgorithm = "bfs";

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Best Branches", s_bestBranches.data(), s_bestBranchesBufferSize);

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
            if (ImGui::CollapsingHeader("Fieldmap Override"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                static int s_tilesPrimaryOverride {512};
                static int s_tilesTotalOverride {1024};
                static int s_metatilesPrimaryOverride {512};
                static int s_metatilesTotalOverride {1024};
                static int s_palsPrimaryOverride {6};
                static int s_palsTotalOverride {13};

                ImGui::TextColored(s_errorTextColor, "Work-in-progress");

                // todo: this should largely be parsed directly from the game files, but maybe you want manual for testing
                // todo: needs a path for fieldmap, button to reload settings, regex to search file for properties
                // todo: needs an alternate option to adjust settings by hand

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
            if (ImGui::CollapsingHeader("Warnings"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                ImGui::TextColored(s_errorTextColor, "Work-in-progress");
                // todo: full warning support. https://github.com/grunt-lucas/porytiles/wiki/Warnings-and-Errors
                // todo: probably implement this as a multi-selection toggle list, with a treat-as-error flag for each option

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
        }

        ImGui::End();
    }
}
