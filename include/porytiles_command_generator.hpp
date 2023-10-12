#ifndef POKEDEV_PORYTILES_COMMAND_GENERATOR_HPP
#define POKEDEV_PORYTILES_COMMAND_GENERATOR_HPP

#include <filesystem>
#include <string>
#include "porytiles_context.hpp"
#include "serializer.hpp"

/// The logic for generating commands that can be run by Porytiles.
class PorytilesCommandGenerator
{
public:
    void renderSettings();

    std::string generateCompilePrimaryCommand(PorytilesContext& context) const;
    std::string generateCompileSecondaryCommand(PorytilesContext& context) const;
    std::string generateDecompilePrimaryCommand(PorytilesContext& context) const;
    std::string generateDecompileSecondaryCommand(PorytilesContext& context) const;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
                CUSTOM_NAME("shouldUseRelativePaths", m_shouldUseRelativePaths),
                CUSTOM_NAME("shouldWslFakeAbsolute", m_shouldWslFakeAbsolute),
                CUSTOM_NAME("relativeBasePath", m_relativeBasePath)
        );
    }

private:
    bool m_shouldUseRelativePaths {};
    bool m_shouldWslFakeAbsolute {};
    std::filesystem::path m_relativeBasePath {};

    std::string getPathString(const std::filesystem::path& path) const;
    std::string getOptions(PorytilesContext& context, const std::filesystem::path& outputPath) const;
};

#endif // POKEDEV_PORYTILES_COMMAND_GENERATOR_HPP
