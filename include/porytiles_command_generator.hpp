#ifndef PORYTILES_GUI_COMMAND_GENERATOR_HPP
#define PORYTILES_GUI_COMMAND_GENERATOR_HPP

#include <filesystem>
#include <string>
#include <cereal/cereal.hpp>
#include "porytiles_context.hpp"

class PorytilesCommandGenerator
{
public:
    void renderImGui();

    std::string generateCompilePrimaryCommand(PorytilesContext& context);
    std::string generateCompileSecondaryCommand(PorytilesContext& context);
    std::string generateDecompilePrimaryCommand(PorytilesContext& context);
    std::string generateDecompileSecondaryCommand(PorytilesContext& context);

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
                cereal::make_nvp("shouldUseRelativePaths", m_shouldUseRelativePaths),
                cereal::make_nvp("shouldWslFakeAbsolute", m_shouldWslFakeAbsolute),
                cereal::make_nvp("relativeBasePath", m_relativeBasePath)
        );
    }

private:
    bool m_shouldUseRelativePaths {};
    bool m_shouldWslFakeAbsolute {};
    std::filesystem::path m_relativeBasePath {};

    std::string getPathString(const std::filesystem::path& path);
    std::string getOptions(PorytilesContext& context, const std::filesystem::path& outputPath);
};

#endif // PORYTILES_GUI_COMMAND_GENERATOR_HPP