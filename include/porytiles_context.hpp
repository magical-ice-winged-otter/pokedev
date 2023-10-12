#ifndef PORYTILES_GUI_CONTEXT
#define PORYTILES_GUI_CONTEXT

#include <filesystem>
#include <string>
#include <cereal/cereal.hpp>

// The working state that Porytiles needs to produce valid results.
// Every field except file paths are given reasonable defaults, closely following the
// porytiles help documentation.
struct PorytilesContext
{
    // General Options
    std::filesystem::path projectPath {};
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
                CEREAL_NVP(projectPath),  /*General*/
                CEREAL_NVP(porytilesExecutableFile),
                CEREAL_NVP(behaviorsHeaderPath),
                CEREAL_NVP(paletteMode),
                CEREAL_NVP(baseGame),
                CEREAL_NVP(useDualLayer),
                cereal::make_nvp("transparency_R", transparency[0]),
                cereal::make_nvp("transparency_G", transparency[1]),
                cereal::make_nvp("transparency_B", transparency[2]),
                CEREAL_NVP(defaultBehavior),
                CEREAL_NVP(assignExploreCutoff),
                CEREAL_NVP(assignAlgorithm),
                CEREAL_NVP(bestBranches),
                CEREAL_NVP(primaryCompileOutputPath), /*Compile Primary*/
                CEREAL_NVP(sourcePrimaryPath),
                CEREAL_NVP(secondaryCompileOutputPath), /*Compile Secondary*/
                CEREAL_NVP(sourceSecondaryPath),
                CEREAL_NVP(sourcePartnerPrimaryPath),
                CEREAL_NVP(primaryAssignExploreCutoff),
                CEREAL_NVP(primaryAssignAlgorithm),
                CEREAL_NVP(primaryBestBranches),
                CEREAL_NVP(primaryDecompileOutputPath), /*Decompile Primary*/
                CEREAL_NVP(compiledPrimaryPath),
                CEREAL_NVP(secondaryDecompileOutputPath),  /*Decompile Secondary*/
                CEREAL_NVP(compiledSecondaryPath),
                CEREAL_NVP(compiledPartnerPrimaryPath)
        );
    }
};

#endif // PORYTILES_GUI_CONTEXT