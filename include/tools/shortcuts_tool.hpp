#ifndef POKEDEV_SHORTCUTS_HPP
#define POKEDEV_SHORTCUTS_HPP

#include <vector>
#include <string>
#include <filesystem>

#include "serializer.hpp"
#include "pokedev_tool.hpp"

struct Shortcut;
struct ShortcutGroup;

const int MAX_GROUPS_PER_SHORTCUT = 100;
const int MAX_SHORTCUTS_PER_GROUP = 100;
const int MAX_GROUPS = 100;
const int MAX_SHORTCUTS = 100;

struct Shortcut {
    std::filesystem::path path {};
    std::string description {};

    int groupCount {};
    ShortcutGroup* groups[MAX_GROUPS_PER_SHORTCUT] {};
};

struct ShortcutGroup {
    std::string name {};

    int memberCount {};
    Shortcut* members[MAX_SHORTCUTS_PER_GROUP] {};
};

class ShortcutsTool : public PokeDevTool {
public:
    ShortcutsTool();
    void renderWindow() override;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(
                AUTO_NAME(shortcutFilePath)
        );
    }
private:
    int groupCount {};
    ShortcutGroup groups[MAX_GROUPS] {};
    int shortcutCount {};
    Shortcut shortcuts[MAX_SHORTCUTS] {};
    std::filesystem::path shortcutFilePath {};
    void init();
};

/*
 * Shortcut file:
 * include/trainers.h, "The trainers that want to battle", [TRAINERS]
 * include/constants/items.h, "Contains item definitions", [ITEMS] [TRAINERS]
 */

#endif
