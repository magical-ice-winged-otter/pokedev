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
    // These are non-optional elements.
    size_t speciesIndex {};
    int level {}; // from 1-100

    // These are optional elements.
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

};

inline std::string generateMonStruct(const TrainerMonData& monData, GameData& gameData)
{
    std::string result = "    {\n";

    result += std::format("        .species = {},\n", gameData.speciesLoader.speciesNames[monData.speciesIndex]);
    result += std::format("        .lvl = {},\n", monData.level);

    if (monData.hasNickname) {
        result += std::format("        .nickname = COMPOUND_STRING({}),\n", monData.nickname);
    }
    if (monData.hasGender) {
        result += std::format("        .gender = TRAINER_MON_{},\n", monData.gender == MonGender::Female ? "FEMALE" : "MALE");
    }
    if (monData.hasFriendship) {
        result += std::format("        .friendship = {},\n", monData.friendship);
    }
    if (monData.hasIsShiny) {
        result += std::format("        .isShiny = {},\n", monData.isShiny ? "TRUE" : "FALSE");
    }
    if (monData.hasEvs) {
        result += std::format("        .ev = TRAINER_PARTY_EVS({}, {}, {}, {}, {}, {}),\n", monData.evs[0], monData.evs[1], monData.evs[2], monData.evs[3], monData.evs[4], monData.evs[5]);
    }
    if (monData.hasIvs) {
        result += std::format("        .ev = TRAINER_PARTY_IVS({}, {}, {}, {}, {}, {}),\n", monData.ivs[0], monData.ivs[1], monData.ivs[2], monData.ivs[3], monData.ivs[4], monData.ivs[5]);
    }
    if (monData.hasAbility) {
        result += std::format("        .ability = {},\n", gameData.abilityLoader.abilityNames[monData.abilityIndex]);
    }
    if (monData.hasBall) {
        result += std::format("        .ball = {},\n", gameData.ballLoader.ballNames[monData.ballIndex]);
    }
    if (monData.hasItem) {
        result += std::format("        .heldItem = {},\n", gameData.itemLoader.itemNames[monData.itemIndex]);
    }
    if (monData.hasNature) {
        result += std::format("        .nature = TRAINER_PARTY_NATURE({}),\n", gameData.natureLoader.natureNames[monData.natureIndex]);
    }
    if (monData.hasMoves) {
        result += std::format("        .moves = {{\n            {},\n            {},\n            {},\n            {}\n        }},\n",
                              gameData.moveLoader.moveNames[monData.movesIndex[0]],
                              gameData.moveLoader.moveNames[monData.movesIndex[1]],
                              gameData.moveLoader.moveNames[monData.movesIndex[2]],
                              gameData.moveLoader.moveNames[monData.movesIndex[3]]
        );
    }

    result += "    }";
    return result;
}
