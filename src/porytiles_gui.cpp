#include <imgui.h>
#include <SDL.h>
#include <SDL_image.h>
#include <imgui_stdlib.h>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "porytiles_gui.hpp"
#include "porytiles_context.hpp"
#include "porytiles_command_generator.hpp"
#include "imgui_utils.hpp"
#include "serializer.hpp"

using namespace std;

// todo: tooltips for everything would be nice...
// todo: a way to reset config / save config / load config would be nice...

namespace PorytilesGui
{
    // General Settings
    static PorytilesContext s_ctx{};
    static PorytilesCommandGenerator s_commandGenerator {};

    // Tools
    static bool s_showPrimaryCompilerTool {};
    static bool s_showPrimaryDecompilerTool {};
    static bool s_showSecondaryDecompilerTool {};
    static bool s_showSecondaryCompilerTool {};
    static filesystem::path s_defaultSourcePath {};
    static filesystem::path s_defaultOutputPath {};

    // GUI Settings
    static ImVec4 s_errorTextColor {1.0, 0.3, 0.3, 1};
    static SDL_Texture* s_previewTexture {};
    static SDL_Renderer* s_renderer {};

    void init(SDL_Renderer* renderer)
    {
        s_renderer = renderer;

        const char* configName = "pokedev_porytiles_gui_config.json";

        if (std::filesystem::exists(configName))
        {
            std::ifstream configFile {configName};
            cereal::JSONInputArchive archive {configFile};

            archive(
                    cereal::make_nvp("porytilesCommandGenerator", s_commandGenerator),
                    cereal::make_nvp("porytilesContext", s_ctx),
                    cereal::make_nvp("showPrimaryCompilerTool", s_showPrimaryCompilerTool),
                    cereal::make_nvp("showPrimaryDecompilerTool", s_showPrimaryDecompilerTool),
                    cereal::make_nvp("showSecondaryCompilerTool", s_showSecondaryCompilerTool),
                    cereal::make_nvp("showSecondaryDecompilerTool", s_showSecondaryDecompilerTool),
                    cereal::make_nvp("defaultOutputPath", s_defaultOutputPath),
                    cereal::make_nvp("defaultSourcePath", s_defaultSourcePath)
            );
        }
    }

    void shutdown()
    {
        const char* configName = "pokedev_porytiles_gui_config.json";
        std::ofstream configFile {configName};
        cereal::JSONOutputArchive archive {configFile};

        archive(
                cereal::make_nvp("porytilesCommandGenerator", s_commandGenerator),
                cereal::make_nvp("porytilesContext", s_ctx),
                cereal::make_nvp("showPrimaryCompilerTool", s_showPrimaryCompilerTool),
                cereal::make_nvp("showPrimaryDecompilerTool", s_showPrimaryDecompilerTool),
                cereal::make_nvp("showSecondaryCompilerTool", s_showSecondaryCompilerTool),
                cereal::make_nvp("showSecondaryDecompilerTool", s_showSecondaryDecompilerTool),
                cereal::make_nvp("defaultOutputPath", s_defaultOutputPath),
                cereal::make_nvp("defaultSourcePath", s_defaultSourcePath)
        );

        SDL_DestroyTexture(s_previewTexture);
    }

    void render()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);

        s_commandGenerator.renderImGui();

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
//            ImGui::ShowDemoWindow();
            if (s_showPrimaryCompilerTool && ImGui::Begin("Primary Compiler", &s_showPrimaryCompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = s_commandGenerator.generateCompilePrimaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiUtils::FolderPicker("Output Path", s_ctx.primaryCompileOutputPath, {.defaultPath = s_defaultOutputPath});
                ImGuiUtils::FolderPicker("Source Primary Path", s_ctx.sourcePrimaryPath, {.defaultPath = s_defaultSourcePath});
                ImGui::SetItemTooltip("Path to a directory containing the source data for a primary set.");

                filesystem::path tilesImagePath {s_ctx.primaryCompileOutputPath / "tiles.png"};

                if (filesystem::exists(tilesImagePath))
                {
                    if (s_previewTexture == nullptr)
                    {
                        s_previewTexture = IMG_LoadTexture(s_renderer, tilesImagePath.string().c_str());
                    }

                    if (ImGui::Button("Reload Preview"))
                    {
                        SDL_DestroyTexture(s_previewTexture);
                        s_previewTexture = nullptr;
                    }

                    Uint32 format;
                    int access, width, height;
                    SDL_QueryTexture(s_previewTexture, &format, &access, &width, &height);
                    ImGui::Image(s_previewTexture, ImVec2{static_cast<float>(width * 2), static_cast<float>(height * 2)});
                }

                ImGui::End();
            }
            if (s_showSecondaryCompilerTool && ImGui::Begin("Secondary Compiler", &s_showSecondaryCompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = s_commandGenerator.generateCompileSecondaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiUtils::FolderPicker("Output Path", s_ctx.secondaryCompileOutputPath, {.defaultPath = s_defaultOutputPath});
                ImGuiUtils::FolderPicker("Source Secondary Path", s_ctx.sourceSecondaryPath, {.defaultPath = s_defaultSourcePath});
                ImGuiUtils::FolderPicker("Source Partner Primary Path", s_ctx.sourcePartnerPrimaryPath, {.defaultPath = s_defaultSourcePath});

                ImGui::SeparatorText("Paired Primary Color Assignment Config");
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("Explore Cutoff", &s_ctx.primaryAssignExploreCutoff);

                if (ImGui::RadioButton("Depth-First Search", s_ctx.primaryAssignAlgorithm == "dfs")) s_ctx.primaryAssignAlgorithm = "dfs"; ImGui::SameLine();
                if (ImGui::RadioButton("Breadth-First Search", s_ctx.primaryAssignAlgorithm == "bfs")) s_ctx.primaryAssignAlgorithm = "bfs";

                ImGui::SetNextItemWidth(150);
                ImGui::InputText("Best Branches", &s_ctx.primaryBestBranches);

                ImGui::End();
            }
            if (s_showPrimaryDecompilerTool && ImGui::Begin("Primary Decompiler", &s_showPrimaryDecompilerTool))
            {
                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = s_commandGenerator.generateDecompilePrimaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiUtils::FolderPicker("Output Path", s_ctx.primaryDecompileOutputPath, {.defaultPath = s_defaultOutputPath});
                ImGuiUtils::FolderPicker("Compiled Primary Path", s_ctx.compiledPrimaryPath, {.defaultPath = s_defaultOutputPath});
                ImGui::End();
            }
            if (s_showSecondaryDecompilerTool && ImGui::Begin("Secondary Decompiler", &s_showSecondaryDecompilerTool))
            {
                ImGui::TextColored(s_errorTextColor, "This feature is not yet supported by Porytiles! Running this command will currently do nothing.");

                if (ImGui::Button("Copy Command to Clipboard"))
                {
                    string result = s_commandGenerator.generateDecompileSecondaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiUtils::FolderPicker("Output Path", s_ctx.secondaryDecompileOutputPath, {.defaultPath = s_defaultSourcePath});
                ImGuiUtils::FolderPicker("Compiled Secondary Path", s_ctx.compiledSecondaryPath, {.defaultPath = s_defaultOutputPath});
                ImGuiUtils::FolderPicker("Compiled Partner Primary Path", s_ctx.compiledPartnerPrimaryPath, {.defaultPath = s_defaultOutputPath});
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

                ImGuiUtils::FolderPicker("Project Path", s_ctx.projectPath, {});
                ImGuiUtils::FilePicker("Porytiles Executable File", s_ctx.porytilesExecutableFile, {});
                ImGuiUtils::FilePicker("Behaviors Header File", s_ctx.behaviorsHeaderPath, {.filter = "h,hpp"});
                ImGuiUtils::FolderPicker("Default Output Path", s_defaultOutputPath, {});
                ImGuiUtils::FolderPicker("Default Source Path", s_defaultSourcePath, {});

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
                ImGui::InputText("Default Behavior", &s_ctx.defaultBehavior);

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
                ImGui::InputText("Best Branches", &s_ctx.bestBranches);

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

} // namespace PorytilesGui
