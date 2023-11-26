#pragma once

#include <filesystem>

class MonAbilityLoader
{
public:
    void init(const std::filesystem::path& abilityFilePath);
    std::vector<std::string> abilityNames {};
};

struct GameData
{
    MonAbilityLoader abilityLoader;
};

GameData loadGameData(const std::filesystem::path& projectPath);