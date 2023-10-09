#include <string>
#include <filesystem>
#include <imgui.h>
#include "porytiles_context.hpp"
#include "porytiles_command_generator.hpp"
#include "serializer.hpp"
#include "imgui_utils.hpp"

using namespace std;

namespace PorytilesCommandGenerator
{
    static bool s_shouldUseRelativePaths {};
    static bool s_shouldWslFakeAbsolute {};
    static filesystem::path s_relativeBasePath {};

    void init()
    {
        Serializer::registerValue("shouldUseRelativePaths", s_shouldUseRelativePaths);
        Serializer::registerValue("relativeBasePath", s_relativeBasePath);
        Serializer::registerValue("shouldWslFakeAbsolute", s_shouldWslFakeAbsolute);
    }

    void shutdown()
    {
        // No shutdown logic needed for now.
    }

    void renderImGui()
    {
        ImGui::SeparatorText("Command Generator");
        ImGui::Checkbox("Should Use Relative Paths", &s_shouldUseRelativePaths);

        if (s_shouldUseRelativePaths)
        {
            ImGuiUtils::FolderPicker("Base Path", s_relativeBasePath);
            ImGui::Checkbox("Should WSL Fake Absolute Path", &s_shouldWslFakeAbsolute);
        }
    }

    static string getPathString(const filesystem::path& path)
    {
        if (s_shouldUseRelativePaths)
        {
            filesystem::path relativePath {filesystem::relative(path, s_relativeBasePath)};

            if (s_shouldWslFakeAbsolute)
                relativePath = filesystem::path{"/"} / relativePath;

            string relativeString = relativePath.string();
            replace(relativeString.begin(), relativeString.end(), '\\', '/');
            return relativeString;
        }
        else return path.string();
    }

    static string getOptions(PorytilesContext& context, const filesystem::path& outputPath)
    {
        string result {};
        result += format(" -output={} ", getPathString(outputPath));
        result += format(" -tiles-output-pal={} ", context.paletteMode);
        result += format(" -target-base-game={} ", context.baseGame);

        if (context.useDualLayer)
            result+= " -dual-layer ";

        result += format(" -transparency-color={},{},{} ", context.transparency[0] * 255, context.transparency[1] * 255, context.transparency[2] * 255);
        result += format(" -default-behavior={} ", context.defaultBehavior);
        result += format(" -assign-explore-cutoff={} ", context.assignExploreCutoff);
        result += format(" -assign-algorithm={} ", context.assignAlgorithm);
        result += format(" -best-branches={} ", context.bestBranches);

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
        return format("{} compile-primary {} {} {}", porytiles, options, srcPrimaryPath, behaviorsHeader);
    }

    string generateCompileSecondaryCommand(PorytilesContext& context)
    {
        string options {getOptions(context, context.secondaryCompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        options += format(" -primary-assign-explore-cutoff={} ", context.primaryAssignExploreCutoff);
        options += format(" -primary-assign-algorithm={} ", context.primaryAssignAlgorithm);
        options += format(" -primary-best-branches={} ", context.primaryBestBranches);

        string srcSecondaryPath {getPathString(context.sourceSecondaryPath)};
        string srcPartnerPrimaryPath {getPathString(context.sourcePartnerPrimaryPath)};
        return format("{} compile-secondary {} {} {} {}", porytiles, options, srcSecondaryPath, srcPartnerPrimaryPath, behaviorsHeader);
    }

    string generateDecompilePrimaryCommand(PorytilesContext& context)
    {
        string options {getOptions(context, context.primaryDecompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        string compiledPrimaryPath {getPathString(context.compiledPrimaryPath)};
        return format("{} decompile-primary {} {} {}", porytiles, options, compiledPrimaryPath, behaviorsHeader);
    }

    string generateDecompileSecondaryCommand(PorytilesContext& context)
    {
        string options {getOptions(context, context.secondaryDecompileOutputPath)};
        string porytiles {getPathString(context.porytilesExecutableFile)};
        string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

        string compiledSecondaryPath {getPathString(context.compiledSecondaryPath)};
        string compiledPartnerPrimaryPath {getPathString(context.compiledPartnerPrimaryPath)};
        return format("{} decompile-secondary {} {} {} {}", porytiles, options, compiledSecondaryPath, compiledPartnerPrimaryPath, behaviorsHeader);
    }

} // namespace PorytilesCommandGenerator
