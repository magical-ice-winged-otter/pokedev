#pragma once

#include <filesystem>

class AbilityLoader
{
public:
    void init(const std::filesystem::path& abilityFilePath);
    std::vector<std::string> abilityNames {};
};

class ItemLoader
{
public:
    void init(const std::filesystem::path& itemFilePath);
    std::vector<std::string> itemNames {};
};

class MoveLoader
{
public:
    void init(const std::filesystem::path& moveFilePath);
    std::vector<std::string> moveNames {};
};

class NatureLoader
{
public:
    void init(const std::filesystem::path& natureFilePath);
    std::vector<std::string> natureNames {};
};

class BallLoader
{
public:
    void init(const std::filesystem::path& ballFilePath);
    std::vector<std::string> ballNames {};
};

class SpeciesLoader
{
public:
    void init(const std::filesystem::path& speciesFilePath);
    std::vector<std::string> speciesNames {};
};

struct GameData
{
    AbilityLoader abilityLoader;
    ItemLoader itemLoader;
    MoveLoader moveLoader;
    NatureLoader natureLoader;
    BallLoader ballLoader;
    SpeciesLoader speciesLoader;
};

GameData loadGameData(const std::filesystem::path& projectPath);