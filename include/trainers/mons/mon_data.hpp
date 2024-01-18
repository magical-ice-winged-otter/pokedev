#pragma once

#include <string>
#include <vector>
#include <filesystem>

enum class MonGender
{
    Male,
    Female,
};

struct MonData
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

    std::string generateStruct() const;
};

