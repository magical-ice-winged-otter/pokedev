#include <fstream>
#include <regex>
#include "game_data.hpp"

GameData loadGameData(const std::filesystem::path &projectPath)
{
    GameData data {};
    data.abilityLoader.init(projectPath / "include/constants/abilities.h");
    data.itemLoader.init(projectPath / "include/constants/items.h");
    data.moveLoader.init(projectPath / "include/constants/moves.h");
    data.natureLoader.init(projectPath / "include/constants/pokemon.h");
    data.ballLoader.init(projectPath / "include/constants/items.h");
    data.speciesLoader.init(projectPath / "include/constants/species.h");
    return data;
}

// todo: this thing deserves to be a util - its too op
static std::vector<std::string> parseDefines(const std::filesystem::path& filePath, const char* regex)
{
    std::ifstream stream {filePath};
    std::string line {};
    std::vector<std::string> results {};
    std::smatch match {};

    while (std::getline(stream, line))
    {
        bool hasDefine = line.find("#define") != std::string::npos;

        if (hasDefine && std::regex_search(line, match, std::regex(regex)))
        {
            size_t start = line.find("#define") + 8;
            size_t end = line.find(' ', start);
            results.emplace_back(line.substr(start, end - start));
        }
    }

    return results;
}

void AbilityLoader::init(const std::filesystem::path& abilityFilePath)
{
    abilityNames = parseDefines(abilityFilePath, ".*#define ABILITY_");
}

void ItemLoader::init(const std::filesystem::path &itemFilePath)
{
    itemNames = parseDefines(itemFilePath, ".*#define ITEM_");
}
void NatureLoader::init(const std::filesystem::path &natureFilePath)
{
    natureNames = parseDefines(natureFilePath, ".*#define NATURE_");
}

void BallLoader::init(const std::filesystem::path &ballFilePath)
{
    ballNames = parseDefines(ballFilePath, ".*#define ITEM_.*_BALL");
}

void MoveLoader::init(const std::filesystem::path &moveFilePath)
{
    moveNames = parseDefines(moveFilePath, ".*#define MOVE_");
}

void SpeciesLoader::init(const std::filesystem::path &speciesFilePath)
{
    speciesNames = parseDefines(speciesFilePath, ".*#define SPECIES_");
}
