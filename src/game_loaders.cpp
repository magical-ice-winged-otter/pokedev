#include <fstream>
#include <regex>
#include "game_loaders.hpp"
#include "string_parsing_util.hpp"

void GameLoaders::init(std::filesystem::path projectPath) {
    m_projectPath = projectPath;
}

#define IMPL_GET_LOADER(loader, name, ...) \
    loader & GameLoaders::get##loader () { \
        if (! m_##name##AreLoaded ) { \
            m_##name .init(__VA_ARGS__); \
            m_##name##AreLoaded = true; \
        } \
        return m_##name ; \
    }\

IMPL_GET_LOADER(AbilityLoader, abilities, m_projectPath / "include/constants/abilities.h")
IMPL_GET_LOADER(ItemLoader, items, m_projectPath / "include/constants/items.h")
IMPL_GET_LOADER(MoveLoader, moves, m_projectPath / "include/constants/moves.h")
IMPL_GET_LOADER(NatureLoader, natures, m_projectPath / "include/constants/pokemon.h")
IMPL_GET_LOADER(BallLoader, balls, m_projectPath / "include/constants/items.h")
IMPL_GET_LOADER(SpeciesLoader, species, m_projectPath / "include/constants/species.h")

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
