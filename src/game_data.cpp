#include "game_data.hpp"

GameData loadGameData(const std::filesystem::path &projectPath)
{
    GameData data {};
    data.abilityLoader.init(projectPath / "include/constants/abilities.h");
    return data;
}
