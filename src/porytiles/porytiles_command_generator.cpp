#include <imgui.h>
#include <filesystem>
#include <string>
#include "imgui_utils.hpp"
#include "porytiles_context.hpp"
#include "porytiles_command_generator.hpp"

using namespace std;

void PorytilesCommandGenerator::renderSettings()
{
    ImGui::SeparatorText("Command Generator");
    ImGui::Checkbox("Should Use Relative Paths", &m_shouldUseRelativePaths);

    if (m_shouldUseRelativePaths)
    {
        ImGuiUtils::folderPicker("Base Path", m_relativeBasePath, {});
        ImGui::Checkbox("Should WSL Fake Absolute Path", &m_shouldWslFakeAbsolute);
    }
}

string PorytilesCommandGenerator::getPathString(const filesystem::path& path) const
{
    if (m_shouldUseRelativePaths)
    {
        filesystem::path relativePath {relative(path, m_relativeBasePath)};

        if (m_shouldWslFakeAbsolute)
            relativePath = filesystem::path{"/"} / relativePath;

        string relativeString = relativePath.string();
        ranges::replace(relativeString, '\\', '/');
        return relativeString;
    }

    return path.string();
}

string PorytilesCommandGenerator::getOptions(PorytilesContext& context, const filesystem::path& outputPath) const
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

string PorytilesCommandGenerator::generateCompilePrimaryCommand(PorytilesContext& context) const
{
    string options {getOptions(context, context.primaryCompileOutputPath)};
    string porytiles {getPathString(context.porytilesExecutableFile)};
    string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

    string srcPrimaryPath {getPathString(context.sourcePrimaryPath)};
    return fmt::format("{} compile-primary {} {} {}", porytiles, options, srcPrimaryPath, behaviorsHeader);
}

string PorytilesCommandGenerator::generateCompileSecondaryCommand(PorytilesContext& context) const
{
    string options {getOptions(context, context.secondaryCompileOutputPath)};
    string porytiles {getPathString(context.porytilesExecutableFile)};
    string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

    options += fmt::format(" -primary-assign-explore-cutoff={} ", context.primaryAssignExploreCutoff);
    options += fmt::format(" -primary-assign-algorithm={} ", context.primaryAssignAlgorithm);
    options += fmt::format(" -primary-best-branches={} ", context.primaryBestBranches);

    string srcSecondaryPath {getPathString(context.sourceSecondaryPath)};
    string srcPartnerPrimaryPath {getPathString(context.sourcePartnerPrimaryPath)};
    return fmt::format("{} compile-secondary {} {} {} {}", porytiles, options, srcSecondaryPath, srcPartnerPrimaryPath, behaviorsHeader);
}

string PorytilesCommandGenerator::generateDecompilePrimaryCommand(PorytilesContext& context) const
{
    string options {getOptions(context, context.primaryDecompileOutputPath)};
    string porytiles {getPathString(context.porytilesExecutableFile)};
    string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

    string compiledPrimaryPath {getPathString(context.compiledPrimaryPath)};
    return fmt::format("{} decompile-primary {} {} {}", porytiles, options, compiledPrimaryPath, behaviorsHeader);
}

string PorytilesCommandGenerator::generateDecompileSecondaryCommand(PorytilesContext& context) const
{
    string options {getOptions(context, context.secondaryDecompileOutputPath)};
    string porytiles {getPathString(context.porytilesExecutableFile)};
    string behaviorsHeader {getPathString(context.behaviorsHeaderPath)};

    string compiledSecondaryPath {getPathString(context.compiledSecondaryPath)};
    string compiledPartnerPrimaryPath {getPathString(context.compiledPartnerPrimaryPath)};
    return fmt::format("{} decompile-secondary {} {} {} {}", porytiles, options, compiledSecondaryPath, compiledPartnerPrimaryPath, behaviorsHeader);
}
