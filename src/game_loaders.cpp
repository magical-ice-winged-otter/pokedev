#include <fstream>
#include <regex>
#include "game_loaders.hpp"
#include "string_parsing_util.hpp"

GameLoaders createLoaders(const std::filesystem::path &projectPath)
{
    GameLoaders loaders {};
    loaders.abilities.init(projectPath / "include/constants/abilities.h");
    loaders.items.init(projectPath / "include/constants/items.h");
    loaders.moves.init(projectPath / "include/constants/moves.h");
    loaders.natures.init(projectPath / "include/constants/pokemon.h");
    loaders.balls.init(projectPath / "include/constants/items.h");
    loaders.species.init(projectPath / "include/constants/species.h");
    return loaders;
}

void AbilityLoader::init(const std::filesystem::path& abilityFilePath) {
    names = parseDefines(abilityFilePath, ".*#define ABILITY_");
}
void ItemLoader::init(const std::filesystem::path &itemFilePath) {
    names = parseDefines(itemFilePath, ".*#define ITEM_");
}
void NatureLoader::init(const std::filesystem::path &natureFilePath) {
    names = parseDefines(natureFilePath, ".*#define NATURE_");
}
void BallLoader::init(const std::filesystem::path &ballFilePath) {
    names = parseDefines(ballFilePath, ".*#define ITEM_.*_BALL");
}
void MoveLoader::init(const std::filesystem::path &moveFilePath) {
    names = parseDefines(moveFilePath, ".*#define MOVE_");
}
void SpeciesLoader::init(const std::filesystem::path &speciesFilePath) {
    names = parseDefines(speciesFilePath, ".*#define SPECIES_");
}
