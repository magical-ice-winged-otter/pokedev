#include <string>
#include <filesystem>
#include <fmt/format.h>
#include <imgui.h>
#include "porytiles_context.hpp"
#include "porytiles_command_generator.hpp"
#include "serializer.hpp"
#include "imgui_utils.hpp"

using namespace std;

namespace PorytilesCommandGenerator
{
    static bool s_shouldUseWslRelativePaths {};
    static filesystem::path s_wslRootPath {};

    void init()
    {
        Serializer::registerValue("shouldUseWslRelativePaths", s_shouldUseWslRelativePaths);
        Serializer::registerValue("wslRootPath", s_wslRootPath);
    }

    void shutdown()
    {
        // No shutdown logic needed for now.
    }

    void renderImGui()
    {
        ImGui::SeparatorText("Command Generator");
        ImGui::Checkbox("Should Use WSL Relative Paths", &s_shouldUseWslRelativePaths);

        if (s_shouldUseWslRelativePaths)
            ImGuiUtils::FolderPicker("WSL Root Path", s_wslRootPath);
    }

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
        string options {getOptions(context, context.primaryDecompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        string compiledPrimaryPath {getPathString(context.compiledPrimaryPath)};
        return fmt::format("{} decompile-primary {} {} {}", porytiles, options, compiledPrimaryPath, behaviorsHeader);
    }

    string generateDecompileSecondaryCommand(PorytilesContext& context)
    {
        string options {getOptions(context, context.secondaryDecompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        string compiledSecondaryPath {getPathString(context.compiledSecondaryPath)};
        string compiledPartnerPrimaryPath {getPathString(context.compiledPartnerPrimaryPath)};
        return fmt::format("{} decompile-secondary {} {} {} {}", porytiles, options, compiledSecondaryPath, compiledPartnerPrimaryPath, behaviorsHeader);
    }

} // namespace PorytilesCommandGenerator
