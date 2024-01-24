#ifndef POKEDEV_GAME_LOADERS_HPP
#define POKEDEV_GAME_LOADERS_HPP

#include <vector>
#include <filesystem>

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

#define DECLARE_GAME_LOADER(loader, name) private: loader m_##name; bool m_##name##AreLoaded; public: loader & get##loader ();

class GameLoaders {
    DECLARE_GAME_LOADER(AbilityLoader, abilities)
    DECLARE_GAME_LOADER(ItemLoader, items)
    DECLARE_GAME_LOADER(MoveLoader, moves)
    DECLARE_GAME_LOADER(NatureLoader, natures)
    DECLARE_GAME_LOADER(BallLoader, balls)
    DECLARE_GAME_LOADER(SpeciesLoader, species)

private:
    std::filesystem::path m_projectPath;
public:
    void init(std::filesystem::path projectPath);
};

#endif