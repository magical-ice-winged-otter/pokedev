#ifndef POKEDEV_PORYTILES_CONTEXT
#define POKEDEV_PORYTILES_CONTEXT

#include <filesystem>
#include <string>
#include "serializer.hpp"

// The working state that Porytiles needs to produce valid results.
// Every field except file paths are given reasonable defaults, closely following the
// porytiles help documentation.
struct PorytilesContext
{
    // General Options
    std::filesystem::path porytilesExecutableFile {};
    std::filesystem::path behaviorsHeaderPath {};
    std::string paletteMode {"greyscale"};
    std::string baseGame {"pokeemerald"};
    bool useDualLayer {false};
    float transparency[3] {1, 0, 1};
    std::string defaultBehavior {"MB_NORMAL"};
    int assignExploreCutoff {2};
    std::string assignAlgorithm {"dfs"};
    std::string bestBranches {"4"};

    // Compile Primary
    std::filesystem::path primaryCompileOutputPath {};
    std::filesystem::path sourcePrimaryPath {};

    // Compile Secondary
    std::filesystem::path secondaryCompileOutputPath {};
    std::filesystem::path sourceSecondaryPath {};
    std::filesystem::path sourcePartnerPrimaryPath {};
    int primaryAssignExploreCutoff {2};
    std::string primaryAssignAlgorithm {"dfs"};
    std::string primaryBestBranches {"4"};

    // Decompile Primary
    std::filesystem::path primaryDecompileOutputPath {};
    std::filesystem::path compiledPrimaryPath {};

    // Decompile Secondary
    std::filesystem::path secondaryDecompileOutputPath {};
    std::filesystem::path compiledSecondaryPath {};
    std::filesystem::path compiledPartnerPrimaryPath {};

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
                AUTO_NAME(porytilesExecutableFile), /*General*/
                AUTO_NAME(behaviorsHeaderPath),
                AUTO_NAME(paletteMode),
                AUTO_NAME(baseGame),
                AUTO_NAME(useDualLayer),
                CUSTOM_NAME("transparency_R", transparency[0]),
                CUSTOM_NAME("transparency_G", transparency[1]),
                CUSTOM_NAME("transparency_B", transparency[2]),
                AUTO_NAME(defaultBehavior),
                AUTO_NAME(assignExploreCutoff),
                AUTO_NAME(assignAlgorithm),
                AUTO_NAME(bestBranches),
                AUTO_NAME(primaryCompileOutputPath), /*Compile Primary*/
                AUTO_NAME(sourcePrimaryPath),
                AUTO_NAME(secondaryCompileOutputPath), /*Compile Secondary*/
                AUTO_NAME(sourceSecondaryPath),
                AUTO_NAME(sourcePartnerPrimaryPath),
                AUTO_NAME(primaryAssignExploreCutoff),
                AUTO_NAME(primaryAssignAlgorithm),
                AUTO_NAME(primaryBestBranches),
                AUTO_NAME(primaryDecompileOutputPath), /*Decompile Primary*/
                AUTO_NAME(compiledPrimaryPath),
                AUTO_NAME(secondaryDecompileOutputPath),  /*Decompile Secondary*/
                AUTO_NAME(compiledSecondaryPath),
                AUTO_NAME(compiledPartnerPrimaryPath)
        );
    }
};

#endif // POKEDEV_PORYTILES_CONTEXT