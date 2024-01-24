#include "application.hpp"
#include "trainers/mons/mon_data.hpp"
#include <fmt/core.h>

std::string MonData::generateStruct() const {
    std::string result = "    {\n";

    result += fmt::format("        .species = {},\n", Application::loaders.getSpeciesLoader().names[speciesIndex]);
    result += fmt::format("        .lvl = {},\n", level);

    if (hasNickname) {
        result += fmt::format("        .nickname = COMPOUND_STRING({}),\n", nickname);
    }
    if (hasGender) {
        result += fmt::format("        .gender = TRAINER_MON_{},\n", gender == MonGender::Female ? "FEMALE" : "MALE");
    }
    if (hasFriendship) {
        result += fmt::format("        .friendship = {},\n", friendship);
    }
    if (hasIsShiny) {
        result += fmt::format("        .isShiny = {},\n", isShiny ? "TRUE" : "FALSE");
    }
    if (hasEvs) {
        result += fmt::format("        .ev = TRAINER_PARTY_EVS({}, {}, {}, {}, {}, {}),\n", evs[0], evs[1], evs[2], evs[3], evs[4], evs[5]);
    }
    if (hasIvs) {
        result += fmt::format("        .ev = TRAINER_PARTY_IVS({}, {}, {}, {}, {}, {}),\n", ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]);
    }
    if (hasAbility) {
        result += fmt::format("        .ability = {},\n", Application::loaders.getAbilityLoader().names[abilityIndex]);
    }
    if (hasBall) {
        result += fmt::format("        .ball = {},\n", Application::loaders.getBallLoader().names[ballIndex]);
    }
    if (hasItem) {
        result += fmt::format("        .heldItem = {},\n", Application::loaders.getItemLoader().names[itemIndex]);
    }
    if (hasNature) {
        result += fmt::format("        .nature = TRAINER_PARTY_NATURE({}),\n", Application::loaders.getNatureLoader().names[natureIndex]);
    }
    if (hasMoves) {
        result += fmt::format("        .moves = {{\n            {},\n            {},\n            {},\n            {}\n        }},\n",
                              Application::loaders.getMoveLoader().names[movesIndex[0]],
                              Application::loaders.getMoveLoader().names[movesIndex[1]],
                              Application::loaders.getMoveLoader().names[movesIndex[2]],
                              Application::loaders.getMoveLoader().names[movesIndex[3]]
        );
    }

    result += "    }";
    return result;
}
