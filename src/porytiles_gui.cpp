#include <imgui.h>
#include <SDL.h>
#include "file_dialog.hpp"
#include "porytiles_gui.hpp"
#include "serializer.hpp"

using namespace std;

// todo: tooltips for everything would be nice...
// todo: a way to reset config / save config / load config would be nice...

struct PorytilesContext
{
    // General Options
    filesystem::path projectPath {};
    filesystem::path behaviorsHeaderPath {};
    string paletteMode {"greyscale"};

    string baseGame {"pokeemerald"};
    bool useDualLayer {false};
    float transparency[3] {1, 0, 1};
    string defaultBehavior {"MB_NORMAL"};

    int assignExploreCutoff {2};
    string assignAlgorithm {"dfs"};
    string bestBranches {"smart"};

    // Compile Primary
    filesystem::path primaryCompileOutputPath {};
    filesystem::path sourcePrimaryPath {};

    // Compile Secondary
    filesystem::path secondaryCompileOutputPath {};
    filesystem::path sourceSecondaryPath {};
    filesystem::path sourcePartnerPrimaryPath {};
    int primaryAssignExploreCutoff {2};
    string primaryAssignAlgorithm {"dfs"};
    string primaryBestBranches {"smart"};

    // Decompile Primary
    filesystem::path primaryDecompileOutputPath {};
    filesystem::path compiledPrimaryPath {};

    // Decompile Secondary
    filesystem::path secondaryDecompileOutputPath {};
    filesystem::path compiledSecondaryPath {};
    filesystem::path compiledPartnerPrimaryPath {};
};

namespace PorytilesCommandGenerator
{
    string generateCompilePrimaryCommand(PorytilesContext& context)
    {
        return "hello primary compile";
    }
    string generateCompileSecondaryCommand(PorytilesContext& context)
    {
        return "hello secondary compile";
    }
    string generateDecompilePrimaryCommand(PorytilesContext& context)
    {
        return "hello primary decompile";
    }
    string generateDecompileSecondaryCommand(PorytilesContext& context)
    {
        return "hello secondary decompile";
    }
}

namespace PorytilesGui
{
    // General Settings
    static PorytilesContext s_ctx{};

    // Tools
    static bool s_showPrimaryCompilerTool {};
    static bool s_showPrimaryDecompilerTool {};
    static bool s_showSecondaryDecompilerTool {};
    static bool s_showSecondaryCompilerTool {};

    // GUI Settings
    static ImVec4 s_subTextColor {0.6, 0.6, 0.6, 1};
    static ImVec4 s_errorTextColor {1.0, 0.3, 0.3, 1};
    static int s_defaultBehaviorBufferSize {100};
    static int s_bestBranchesBufferSize {100};
    static int s_primaryBestBranchesBufferSize {100};

    void init()
    {
        s_ctx.defaultBehavior.resize(s_defaultBehaviorBufferSize);
        s_ctx.bestBranches.resize(s_bestBranchesBufferSize);
        s_ctx.primaryBestBranches.resize(s_primaryBestBranchesBufferSize);

        Serializer::registerValue("assignExploreCutoff", s_ctx.assignExploreCutoff);
        Serializer::registerValue("assignAlgorithm", s_ctx.assignAlgorithm);
        Serializer::registerValue("bestBranches", s_ctx.bestBranches);
        Serializer::registerValue("primaryAssignExploreCutoff", s_ctx.primaryAssignExploreCutoff);
        Serializer::registerValue("primaryAssignAlgorithm", s_ctx.primaryAssignAlgorithm);
        Serializer::registerValue("primaryBestBranches", s_ctx.primaryBestBranches);

        Serializer::registerValue("transparencyColor_R", s_ctx.transparency[0]);
        Serializer::registerValue("transparencyColor_G", s_ctx.transparency[1]);
        Serializer::registerValue("transparencyColor_B", s_ctx.transparency[2]);
        Serializer::registerValue("defaultBehavior", s_ctx.defaultBehavior);
        Serializer::registerValue("useDualLayer", s_ctx.useDualLayer);
        Serializer::registerValue("baseGame", s_ctx.baseGame);

        Serializer::registerValue("projectPath", s_ctx.projectPath);
        Serializer::registerValue("behaviorsHeaderPath", s_ctx.behaviorsHeaderPath);
        Serializer::registerValue("paletteMode", s_ctx.paletteMode);

        Serializer::registerValue("showPrimaryCompiler", s_showPrimaryCompilerTool);
        Serializer::registerValue("showPrimaryDecompiler", s_showPrimaryDecompilerTool);
        Serializer::registerValue("showSecondaryCompiler", s_showSecondaryCompilerTool);
        Serializer::registerValue("showSecondaryDecompiler", s_showSecondaryDecompilerTool);

        Serializer::registerValue("primaryCompileOutputPath", s_ctx.primaryCompileOutputPath);
        Serializer::registerValue("primaryDecompileOutputPath", s_ctx.primaryDecompileOutputPath);
        Serializer::registerValue("secondaryCompileOutputPath", s_ctx.secondaryCompileOutputPath);
        Serializer::registerValue("secondaryDecompileOutputPath", s_ctx.secondaryDecompileOutputPath);
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
                if (ImGui::BeginMenu("File"))
                {
                    // todo: implement
                    ImGui::MenuItem("Save Config As");
                    // todo: implement
                    ImGui::MenuItem("Load Config");
                    ImGui::EndMenu();
                }

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
                    string result = PorytilesCommandGenerator::generateCompilePrimaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiFolderPicker("Output Path", s_ctx.primaryCompileOutputPath);
                ImGuiFolderPicker("Source Primary Path", s_ctx.sourcePrimaryPath);
                ImGui::SetItemTooltip("Path to a directory containing the source data for a primary set.");
                ImGui::End();
            }
            if (s_showSecondaryCompilerTool && ImGui::Begin("Secondary Compiler", &s_showSecondaryCompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = PorytilesCommandGenerator::generateCompileSecondaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiFolderPicker("Output Path", s_ctx.secondaryCompileOutputPath);
                ImGuiFolderPicker("Compiled Primary Path", s_ctx.compiledPrimaryPath);

                ImGui::SeparatorText("Paired Primary Color Assignment Config");
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("Explore Cutoff", &s_ctx.primaryAssignExploreCutoff);

                if (ImGui::RadioButton("Depth-First Search", s_ctx.primaryAssignAlgorithm == "dfs")) s_ctx.primaryAssignAlgorithm = "dfs"; ImGui::SameLine();
                if (ImGui::RadioButton("Breadth-First Search", s_ctx.primaryAssignAlgorithm == "bfs")) s_ctx.primaryAssignAlgorithm = "bfs";

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Best Branches", s_ctx.primaryBestBranches.data(), s_primaryBestBranchesBufferSize);

                ImGui::End();
            }
            if (s_showPrimaryDecompilerTool && ImGui::Begin("Primary Decompiler", &s_showPrimaryDecompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = PorytilesCommandGenerator::generateDecompilePrimaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiFolderPicker("Output Path", s_ctx.primaryDecompileOutputPath);
                ImGuiFolderPicker("Source Secondary Path", s_ctx.sourceSecondaryPath);
                ImGuiFolderPicker("Source Partner Primary Path", s_ctx.sourceSecondaryPath);
                ImGui::End();
            }
            if (s_showSecondaryDecompilerTool && ImGui::Begin("Secondary Decompiler", &s_showSecondaryDecompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = PorytilesCommandGenerator::generateDecompileSecondaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiFolderPicker("Output Path", s_ctx.secondaryDecompileOutputPath);
                ImGuiFolderPicker("Compiled Secondary Path", s_ctx.compiledSecondaryPath);
                ImGuiFolderPicker("Compiled Partner Primary Path", s_ctx.compiledPartnerPrimaryPath);
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

                ImGuiFolderPicker("Project Path", s_ctx.projectPath);
                ImGuiFilePicker("Behaviors Header File", s_ctx.behaviorsHeaderPath, "h,hpp");

                ImGui::Text("Palette Mode");
                if (ImGui::RadioButton("True Color", s_ctx.paletteMode == "true-color")) s_ctx.paletteMode = "true-color"; ImGui::SameLine();
                if (ImGui::RadioButton("Greyscale", s_ctx.paletteMode == "greyscale")) s_ctx.paletteMode = "greyscale";

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
            if (ImGui::CollapsingHeader("Tileset (De)compilation"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                if (ImGui::RadioButton("Emerald", s_ctx.baseGame == "pokeemerald")) s_ctx.baseGame = "pokeemerald"; ImGui::SameLine();
                if (ImGui::RadioButton("Fire Red", s_ctx.baseGame == "pokefirered")) s_ctx.baseGame = "pokefirered"; ImGui::SameLine();
                if (ImGui::RadioButton("Ruby", s_ctx.baseGame == "pokeruby")) s_ctx.baseGame = "pokeruby";

                if (ImGui::RadioButton("Dual", s_ctx.useDualLayer)) s_ctx.useDualLayer = true; ImGui::SameLine();
                if (ImGui::RadioButton("Triple", !s_ctx.useDualLayer)) s_ctx.useDualLayer = false;

                ImGui::SetNextItemWidth(300);
                ImGui::ColorEdit3("Transparency Color", s_ctx.transparency);

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Default Behavior", s_ctx.defaultBehavior.data(), s_defaultBehaviorBufferSize);

                ImGui::Unindent();
                ImGui::Spacing(); ImGui::Spacing();
            }
            if (ImGui::CollapsingHeader("Color assignment Config"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("Assign Explore Cutoff", &s_ctx.assignExploreCutoff);

                if (ImGui::RadioButton("Depth-First Search", s_ctx.assignAlgorithm == "dfs")) s_ctx.assignAlgorithm = "dfs"; ImGui::SameLine();
                if (ImGui::RadioButton("Breadth-First Search", s_ctx.assignAlgorithm == "bfs")) s_ctx.assignAlgorithm = "bfs";

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Best Branches", s_ctx.bestBranches.data(), s_bestBranchesBufferSize);

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
