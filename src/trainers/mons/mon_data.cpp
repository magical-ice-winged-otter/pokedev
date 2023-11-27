#include "application.hpp"
#include "mon_data.hpp"

std::string MonData::generateStruct() const {
    std::string result = "    {\n";

    result += std::format("        .species = {},\n", Application::loaders.species.names[speciesIndex]);
    result += std::format("        .lvl = {},\n", level);

    if (hasNickname) {
        result += std::format("        .nickname = COMPOUND_STRING({}),\n", nickname);
    }
    if (hasGender) {
        result += std::format("        .gender = TRAINER_MON_{},\n", gender == MonGender::Female ? "FEMALE" : "MALE");
    }
    if (hasFriendship) {
        result += std::format("        .friendship = {},\n", friendship);
    }
    if (hasIsShiny) {
        result += std::format("        .isShiny = {},\n", isShiny ? "TRUE" : "FALSE");
    }
    if (hasEvs) {
        result += std::format("        .ev = TRAINER_PARTY_EVS({}, {}, {}, {}, {}, {}),\n", evs[0], evs[1], evs[2], evs[3], evs[4], evs[5]);
    }
    if (hasIvs) {
        result += std::format("        .ev = TRAINER_PARTY_IVS({}, {}, {}, {}, {}, {}),\n", ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]);
    }
    if (hasAbility) {
        result += std::format("        .ability = {},\n", Application::loaders.abilities.names[abilityIndex]);
    }
    if (hasBall) {
        result += std::format("        .ball = {},\n", Application::loaders.balls.names[ballIndex]);
    }
    if (hasItem) {
        result += std::format("        .heldItem = {},\n", Application::loaders.items.names[itemIndex]);
    }
    if (hasNature) {
        result += std::format("        .nature = TRAINER_PARTY_NATURE({}),\n", Application::loaders.natures.names[natureIndex]);
    }
    if (hasMoves) {
        result += std::format("        .moves = {{\n            {},\n            {},\n            {},\n            {}\n        }},\n",
                              Application::loaders.moves.names[movesIndex[0]],
                              Application::loaders.moves.names[movesIndex[1]],
                              Application::loaders.moves.names[movesIndex[2]],
                              Application::loaders.moves.names[movesIndex[3]]
        );
    }

    result += "    }";
    return result;
}
