#include <imgui.h>
#include <SDL.h>
#include <SDL_image.h>
#include <fmt/format.h>
#include <imgui_stdlib.h>
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
    filesystem::path porytilesExecutableFile {};
    filesystem::path behaviorsHeaderPath {};
    string paletteMode {"greyscale"};

    string baseGame {"pokeemerald"};
    bool useDualLayer {false};
    float transparency[3] {1, 0, 1};
    string defaultBehavior {"MB_NORMAL"};

    int assignExploreCutoff {2};
    string assignAlgorithm {"dfs"};
    string bestBranches {"4"};

    // Compile Primary
    filesystem::path primaryCompileOutputPath {};
    filesystem::path sourcePrimaryPath {};

    // Compile Secondary
    filesystem::path secondaryCompileOutputPath {};
    filesystem::path sourceSecondaryPath {};
    filesystem::path sourcePartnerPrimaryPath {};
    int primaryAssignExploreCutoff {2};
    string primaryAssignAlgorithm {"dfs"};
    string primaryBestBranches {"4"};

    // Decompile Primary
    filesystem::path primaryDecompileOutputPath {};
    filesystem::path compiledPrimaryPath {};

    // Decompile Secondary
    filesystem::path secondaryDecompileOutputPath {};
    filesystem::path compiledSecondaryPath {};
    filesystem::path compiledPartnerPrimaryPath {};
};

static bool s_shouldUseWslRelativePaths {};
static filesystem::path s_wslRootPath {};

namespace PorytilesCommandGenerator
{
    static string getPathString(const filesystem::path& path)
    {
        if (s_shouldUseWslRelativePaths)
        {
            filesystem::path relativePath {filesystem::path{"/"} / filesystem::relative(path, s_wslRootPath)};
            string relativeString = relativePath.string();
            replace(relativeString.begin(), relativeString.end(), '\\', '/');
            return relativeString;
        }
        else return path.string();
    }

    static string getOptions(PorytilesContext& context, const filesystem::path& outputPath)
    {
        string result {};
        result += fmt::format(" -output={} ", getPathString(outputPath));
        result += fmt::format(" -tiles-output-pal={} ", context.paletteMode);
        result += fmt::format(" -target-base-game={} ", context.baseGame);

        if (context.useDualLayer)
            result+= " -dual-layer ";

        result += fmt::format(" -transparency-color={},{},{} ", context.transparency[0] * 255, context.transparency[1] * 255, context.transparency[2] * 255);
        result += fmt::format(" -default-behavior={} ", context.defaultBehavior);
        result += fmt::format(" -assign-explore-cutoff={} ", context.assignExploreCutoff);
        result += fmt::format(" -assign-algorithm={} ", context.assignAlgorithm);
        result += fmt::format(" -best-branches={} ", context.bestBranches);

        // todo: fieldmap options
        // todo: warning options

        return result;
    }

    string generateCompilePrimaryCommand(PorytilesContext& context)
    {
        string options {getOptions(context, context.primaryCompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        string srcPrimaryPath {getPathString(context.sourcePrimaryPath)};
        return fmt::format("{} compile-primary {} {} {}", porytiles, options, srcPrimaryPath, behaviorsHeader);
    }

    string generateCompileSecondaryCommand(PorytilesContext& context)
    {
        string options {getOptions(context, context.secondaryCompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        string srcSecondaryPath {getPathString(context.sourceSecondaryPath)};
        string srcPartnerPrimaryPath {getPathString(context.sourcePartnerPrimaryPath)};
        return fmt::format("{} compile-secondary {} {} {} {}", porytiles, options, srcSecondaryPath, srcPartnerPrimaryPath, behaviorsHeader);
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
    static ImVec4 s_errorTextColor {1.0, 0.3, 0.3, 1};
    static SDL_Texture* s_previewTexture {};
    static SDL_Renderer* s_renderer {};

    void init(SDL_Renderer* renderer)
    {
        s_renderer = renderer;

        // Options
        Serializer::registerValue("assignExploreCutoff", s_ctx.assignExploreCutoff);
        Serializer::registerValue("assignAlgorithm", s_ctx.assignAlgorithm);
        Serializer::registerValue("bestBranches", s_ctx.bestBranches);

        Serializer::registerValue("transparencyColor_R", s_ctx.transparency[0]);
        Serializer::registerValue("transparencyColor_G", s_ctx.transparency[1]);
        Serializer::registerValue("transparencyColor_B", s_ctx.transparency[2]);
        Serializer::registerValue("defaultBehavior", s_ctx.defaultBehavior);
        Serializer::registerValue("useDualLayer", s_ctx.useDualLayer);
        Serializer::registerValue("baseGame", s_ctx.baseGame);

        Serializer::registerValue("projectPath", s_ctx.projectPath);
        Serializer::registerValue("porytilesExecutableFile", s_ctx.porytilesExecutableFile);
        Serializer::registerValue("behaviorsHeaderPath", s_ctx.behaviorsHeaderPath);
        Serializer::registerValue("paletteMode", s_ctx.paletteMode);

        // Primary compiler
        Serializer::registerValue("primaryCompileOutputPath", s_ctx.primaryCompileOutputPath);
        Serializer::registerValue("sourcePrimaryPath", s_ctx.sourcePrimaryPath);

        // Secondary compiler
        Serializer::registerValue("secondaryCompileOutputPath", s_ctx.secondaryCompileOutputPath);
        Serializer::registerValue("sourceSecondaryPath", s_ctx.sourceSecondaryPath);
        Serializer::registerValue("sourcePartnerPrimaryPath", s_ctx.sourcePartnerPrimaryPath);
        Serializer::registerValue("primaryAssignExploreCutoff", s_ctx.primaryAssignExploreCutoff);
        Serializer::registerValue("primaryAssignAlgorithm", s_ctx.primaryAssignAlgorithm);
        Serializer::registerValue("primaryBestBranches", s_ctx.primaryBestBranches);

        // Primary decompiler
        Serializer::registerValue("primaryDecompileOutputPath", s_ctx.primaryDecompileOutputPath);
        Serializer::registerValue("compiledPrimaryPath", s_ctx.compiledPrimaryPath);

        // Secondary decompiler
        Serializer::registerValue("secondaryDecompileOutputPath", s_ctx.secondaryDecompileOutputPath);
        Serializer::registerValue("compiledSecondaryPath", s_ctx.compiledSecondaryPath);
        Serializer::registerValue("compiledPartnerPrimaryPath", s_ctx.compiledPartnerPrimaryPath);

        // Gui settings
        Serializer::registerValue("shouldUseWslRelativePaths", s_shouldUseWslRelativePaths);
        Serializer::registerValue("wslRootPath", s_wslRootPath);
        Serializer::registerValue("showPrimaryCompiler", s_showPrimaryCompilerTool);
        Serializer::registerValue("showPrimaryDecompiler", s_showPrimaryDecompilerTool);
        Serializer::registerValue("showSecondaryCompiler", s_showSecondaryCompilerTool);
        Serializer::registerValue("showSecondaryDecompiler", s_showSecondaryDecompilerTool);
    }

    void shutdown()
    {
        SDL_DestroyTexture(s_previewTexture);
    }

    // todo: move this into custom ImGui utils file?
    static void ImGuiFolderPicker(const char* label, filesystem::path& path)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(fmt::format("Edit##{}", label).c_str()))
            FileDialog::tryPickFolder(path);

        ImGui::SameLine();
        string pathString {path.string()};
        ImGui::SetNextItemWidth(500);

        if (ImGui::InputTextWithHint(fmt::format("##{}", label).c_str(), "No folder selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

    // todo: move this into custom ImGui utils file?
    static void ImGuiFilePicker(const char* label, filesystem::path& path, const char* filter = nullptr)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(fmt::format("Edit##{}", label).c_str()))
            FileDialog::tryPickFile(path, filter);

        ImGui::SameLine();
        string pathString {path.string()};
        ImGui::SetNextItemWidth(500);

        if (ImGui::InputTextWithHint(fmt::format("##{}", label).c_str(), "No file selected", &pathString))
            path = filesystem::path {pathString};

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
//            ImGui::ShowDemoWindow();
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
                    string result = PorytilesCommandGenerator::generateCompileSecondaryCommand(s_ctx);
                    SDL_SetClipboardText(result.c_str());
                }

                ImGuiFolderPicker("Output Path", s_ctx.secondaryCompileOutputPath);
                ImGuiFolderPicker("Source Secondary Path", s_ctx.sourceSecondaryPath);
                ImGuiFolderPicker("Source Partner Primary Path", s_ctx.sourcePartnerPrimaryPath);

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

            ImGui::Checkbox("Should Use WSL Relative Paths", &s_shouldUseWslRelativePaths);

            if (s_shouldUseWslRelativePaths)
            {
                ImGui::Text("%s", "Relative Path Base");

                if (ImGui::Button("Edit"))
                    FileDialog::tryPickFolder(s_wslRootPath);

                ImGui::SameLine();
                string pathString {s_wslRootPath.string()};
                ImGui::SetNextItemWidth(500);

                if (ImGui::InputTextWithHint("##wslBasePath", "No file selected", &pathString))
                    s_wslRootPath = filesystem::path {pathString};

                ImGui::Spacing(); ImGui::Spacing();
            }

            if (ImGui::CollapsingHeader("Driver"))
            {
                ImGui::Spacing(); ImGui::Spacing();
                ImGui::Indent();

                ImGuiFolderPicker("Project Path", s_ctx.projectPath);
                ImGuiFilePicker("Porytiles Executable File", s_ctx.porytilesExecutableFile);
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
}
