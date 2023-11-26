#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "game_data.hpp"

enum class MonGender
{
    Male,
    Female,
};

struct TrainerMonData
{
    bool hasAbility {false};
    size_t abilityIndex {};

    bool hasItem {false};
    size_t itemIndex {};

    bool hasMoves {false};
    size_t movesIndex[4] {};

    bool hasNature {false};
    size_t natureIndex {};

    bool hasBall {false};
    size_t ballIndex {};

    bool hasSpecies {false};
    size_t speciesIndex {};

    bool hasGender {false};
    MonGender gender {};

    bool hasNickname {false};
    std::string nickname {};

    bool hasIsShiny {false};
    bool isShiny {};

    bool hasEvs {false};
    int evs[8] {}; // from 0-252

    bool hasIvs {false};
    int ivs[8] {}; // from 0-252

    bool hasFriendship {false};
    int friendship {}; // from 0-255

    bool hasLevel {false};
    int level {}; // from 1-100
};

inline std::string generateMonStruct(const TrainerMonData& monData, GameData& gameData)
{
    std::string result = "    {";

    if (monData.hasAbility)
        result += std::format("        .ability = {}", gameData.abilityLoader.abilityNames[monData.abilityIndex]);

    result += "    }";
    return result;
}
