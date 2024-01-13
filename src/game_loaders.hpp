#ifndef POKEDEV_GAME_LOADERS_HPP
#define POKEDEV_GAME_LOADERS_HPP

#ifdef _WIN32
#include <filesystem>
#else
#include <vector>
#include <filesystem>
#endif

class AbilityLoader {
public:
    void init(const std::filesystem::path& abilityFilePath);
    std::vector<std::string> names {};
};

class ItemLoader {
public:
    void init(const std::filesystem::path& itemFilePath);
    std::vector<std::string> names {};
};

class MoveLoader {
public:
    void init(const std::filesystem::path& moveFilePath);
    std::vector<std::string> names {};
};

class NatureLoader {
public:
    void init(const std::filesystem::path& natureFilePath);
    std::vector<std::string> names {};
};

class BallLoader {
public:
    void init(const std::filesystem::path& ballFilePath);
    std::vector<std::string> names {};
};

class SpeciesLoader {
public:
    void init(const std::filesystem::path& speciesFilePath);
    std::vector<std::string> names {};
};

struct GameLoaders {
    AbilityLoader abilities;
    ItemLoader items;
    MoveLoader moves;
    NatureLoader natures;
    BallLoader balls;
    SpeciesLoader species;
};

GameLoaders createLoaders(const std::filesystem::path& projectPath);

#endif