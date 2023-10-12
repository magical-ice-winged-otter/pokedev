#include <imgui.h>
#include <SDL.h>
#include <SDL_image.h>
#include <imgui_stdlib.h>
#include "porytiles_gui.hpp"
#include "imgui_utils.hpp"

using namespace std;

// todo: tooltips for everything would be nice...
// todo: a way to reset config / save config / load config would be nice...
// todo: making this class smaller? the windows might be able to be split, more texture preview stuff...

// General Settings

void PorytilesGui::init(SDL_Renderer* renderer)
{
    m_renderer = renderer;
}

void PorytilesGui::shutdown()
{
    SDL_DestroyTexture(m_previewTexture);
}

void PorytilesGui::render()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);

    m_commandGenerator.renderImGui();

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
                ImGui::MenuItem("Primary Compiler", nullptr, &m_showPrimaryCompilerTool);
                ImGui::MenuItem("Primary Decompiler", nullptr, &m_showPrimaryDecompilerTool);
                ImGui::MenuItem("Secondary Compiler", nullptr, &m_showSecondaryCompilerTool);
                ImGui::MenuItem("Secondary Decompiler", nullptr, &m_showSecondaryDecompilerTool);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    // Windows
    {
//            ImGui::ShowDemoWindow();
        if (m_showPrimaryCompilerTool && ImGui::Begin("Primary Compiler", &m_showPrimaryCompilerTool))
        {
            if (ImGui::Button("Copy Command to Clipboard"))
            {
                string result = m_commandGenerator.generateCompilePrimaryCommand(m_ctx);
                SDL_SetClipboardText(result.c_str());
            }

            ImGuiUtils::FolderPicker("Output Path", m_ctx.primaryCompileOutputPath, {.defaultPath = m_defaultOutputPath});
            ImGuiUtils::FolderPicker("Source Primary Path", m_ctx.sourcePrimaryPath, {.defaultPath = m_defaultSourcePath});
            ImGui::SetItemTooltip("Path to a directory containing the source data for a primary set.");

            filesystem::path tilesImagePath {m_ctx.primaryCompileOutputPath / "tiles.png"};

            if (filesystem::exists(tilesImagePath))
            {
                if (m_previewTexture == nullptr)
                {
                    m_previewTexture = IMG_LoadTexture(m_renderer, tilesImagePath.string().c_str());
                }

                if (ImGui::Button("Reload Preview"))
                {
                    SDL_DestroyTexture(m_previewTexture);
                    m_previewTexture = nullptr;
                }

                Uint32 format;
                int access, width, height;
                SDL_QueryTexture(m_previewTexture, &format, &access, &width, &height);
                ImGui::Image(m_previewTexture, ImVec2{static_cast<float>(width * 2), static_cast<float>(height * 2)});
            }

            ImGui::End();
        }
        if (m_showSecondaryCompilerTool && ImGui::Begin("Secondary Compiler", &m_showSecondaryCompilerTool))
        {
            if (ImGui::Button("Copy Command to Clipboard"))
            {
                string result = m_commandGenerator.generateCompileSecondaryCommand(m_ctx);
                SDL_SetClipboardText(result.c_str());
            }

            ImGuiUtils::FolderPicker("Output Path", m_ctx.secondaryCompileOutputPath, {.defaultPath = m_defaultOutputPath});
            ImGuiUtils::FolderPicker("Source Secondary Path", m_ctx.sourceSecondaryPath, {.defaultPath = m_defaultSourcePath});
            ImGuiUtils::FolderPicker("Source Partner Primary Path", m_ctx.sourcePartnerPrimaryPath, {.defaultPath = m_defaultSourcePath});

            ImGui::SeparatorText("Paired Primary Color Assignment Config");
            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("Explore Cutoff", &m_ctx.primaryAssignExploreCutoff);

            if (ImGui::RadioButton("Depth-First Search", m_ctx.primaryAssignAlgorithm == "dfs")) m_ctx.primaryAssignAlgorithm = "dfs"; ImGui::SameLine();
            if (ImGui::RadioButton("Breadth-First Search", m_ctx.primaryAssignAlgorithm == "bfs")) m_ctx.primaryAssignAlgorithm = "bfs";

            ImGui::SetNextItemWidth(150);
            ImGui::InputText("Best Branches", &m_ctx.primaryBestBranches);

            ImGui::End();
        }
        if (m_showPrimaryDecompilerTool && ImGui::Begin("Primary Decompiler", &m_showPrimaryDecompilerTool))
        {
            if (ImGui::Button("Copy Command to Clipboard"))
            {
                string result = m_commandGenerator.generateDecompilePrimaryCommand(m_ctx);
                SDL_SetClipboardText(result.c_str());
            }

            ImGuiUtils::FolderPicker("Output Path", m_ctx.primaryDecompileOutputPath, {.defaultPath = m_defaultOutputPath});
            ImGuiUtils::FolderPicker("Compiled Primary Path", m_ctx.compiledPrimaryPath, {.defaultPath = m_defaultOutputPath});
            ImGui::End();
        }
        if (m_showSecondaryDecompilerTool && ImGui::Begin("Secondary Decompiler", &m_showSecondaryDecompilerTool))
        {
            ImGui::TextColored(m_errorTextColor, "This feature is not yet supported by Porytiles! Running this command will currently do nothing.");

            if (ImGui::Button("Copy Command to Clipboard"))
            {
                string result = m_commandGenerator.generateDecompileSecondaryCommand(m_ctx);
                SDL_SetClipboardText(result.c_str());
            }

            ImGuiUtils::FolderPicker("Output Path", m_ctx.secondaryDecompileOutputPath, {.defaultPath = m_defaultSourcePath});
            ImGuiUtils::FolderPicker("Compiled Secondary Path", m_ctx.compiledSecondaryPath, {.defaultPath = m_defaultOutputPath});
            ImGuiUtils::FolderPicker("Compiled Partner Primary Path", m_ctx.compiledPartnerPrimaryPath, {.defaultPath = m_defaultOutputPath});
            ImGui::End();
        }
    }

    // Tools
    {
        ImGui::SeparatorText("Tools");

        ImGui::Text("Primary"); ImGui::SameLine();
        m_showPrimaryCompilerTool |= ImGui::Button("Compiler##Primary"); ImGui::SameLine();
        m_showPrimaryDecompilerTool |= ImGui::Button("Decompiler##Primary");

        ImGui::Text("Secondary"); ImGui::SameLine();
        m_showSecondaryCompilerTool |= ImGui::Button("Compiler##Secondary"); ImGui::SameLine();
        m_showSecondaryDecompilerTool |= ImGui::Button("Decompiler##Secondary");
    }

    // Settings
    {
        ImGui::SeparatorText("Settings");

        if (ImGui::CollapsingHeader("Driver"))
        {
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::Indent();

            ImGuiUtils::FolderPicker("Project Path", m_ctx.projectPath, {});
            ImGuiUtils::FilePicker("Porytiles Executable File", m_ctx.porytilesExecutableFile, {});
            ImGuiUtils::FilePicker("Behaviors Header File", m_ctx.behaviorsHeaderPath, {.filter = "h,hpp"});
            ImGuiUtils::FolderPicker("Default Output Path", m_defaultOutputPath, {});
            ImGuiUtils::FolderPicker("Default Source Path", m_defaultSourcePath, {});

            ImGui::Text("Palette Mode");
            if (ImGui::RadioButton("True Color", m_ctx.paletteMode == "true-color")) m_ctx.paletteMode = "true-color"; ImGui::SameLine();
            if (ImGui::RadioButton("Greyscale", m_ctx.paletteMode == "greyscale")) m_ctx.paletteMode = "greyscale";

            ImGui::Unindent();
            ImGui::Spacing(); ImGui::Spacing();
        }
        if (ImGui::CollapsingHeader("Tileset (De)compilation"))
        {
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::Indent();

            if (ImGui::RadioButton("Emerald", m_ctx.baseGame == "pokeemerald")) m_ctx.baseGame = "pokeemerald"; ImGui::SameLine();
            if (ImGui::RadioButton("Fire Red", m_ctx.baseGame == "pokefirered")) m_ctx.baseGame = "pokefirered"; ImGui::SameLine();
            if (ImGui::RadioButton("Ruby", m_ctx.baseGame == "pokeruby")) m_ctx.baseGame = "pokeruby";

            if (ImGui::RadioButton("Dual", m_ctx.useDualLayer)) m_ctx.useDualLayer = true; ImGui::SameLine();
            if (ImGui::RadioButton("Triple", !m_ctx.useDualLayer)) m_ctx.useDualLayer = false;

            ImGui::SetNextItemWidth(300);
            ImGui::ColorEdit3("Transparency Color", m_ctx.transparency);

            ImGui::SetNextItemWidth(150);
            ImGui::InputText("Default Behavior", &m_ctx.defaultBehavior);

            ImGui::Unindent();
            ImGui::Spacing(); ImGui::Spacing();
        }
        if (ImGui::CollapsingHeader("Color assignment Config"))
        {
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::Indent();

            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("Assign Explore Cutoff", &m_ctx.assignExploreCutoff);

            if (ImGui::RadioButton("Depth-First Search", m_ctx.assignAlgorithm == "dfs")) m_ctx.assignAlgorithm = "dfs"; ImGui::SameLine();
            if (ImGui::RadioButton("Breadth-First Search", m_ctx.assignAlgorithm == "bfs")) m_ctx.assignAlgorithm = "bfs";

            ImGui::SetNextItemWidth(150);
            ImGui::InputText("Best Branches", &m_ctx.bestBranches);

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

            ImGui::TextColored(m_errorTextColor, "Work-in-progress");

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

            ImGui::TextColored(m_errorTextColor, "Work-in-progress");
            // todo: full warning support. https://github.com/grunt-lucas/porytiles/wiki/Warnings-and-Errors
            // todo: probably implement this as a multi-selection toggle list, with a treat-as-error flag for each option

            ImGui::Unindent();
            ImGui::Spacing(); ImGui::Spacing();
        }
    }

    ImGui::End();
}
