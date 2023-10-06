#ifndef PORYTILES_GUI_CONTEXT
#define PORYTILES_GUI_CONTEXT

#include <filesystem>
#include <string>

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
};

#endif // PORYTILES_GUI_CONTEXT