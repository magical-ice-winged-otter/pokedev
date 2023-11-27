#include <fstream>
#include <string>
#include <imgui.h>
#include "shortcuts.hpp"
#include "string_parsing_util.hpp"
#include "platform.hpp"

ShortcutGui ShortcutGui::loadFromFile(const std::filesystem::path &file) {
    std::ifstream shortcutFileStream {file};
    std::string line {};
    ShortcutGui result {};

    while (std::getline(shortcutFileStream, line)) {
        Shortcut& shortcut = result.shortcuts[result.shortcutCount];
        result.shortcutCount++;

        std::string rawPath;
        std::string rawGroups;

        tryGetCsvColumn(line, 0, rawPath);
        tryGetCsvColumn(line, 1, shortcut.description);
        tryGetCsvColumn(line, 2, rawGroups);

        shortcut.path = rawPath;

        // Now, extract the raw group names from the final column and parse them.
        std::vector<std::string> groupNames = split(rawGroups, "|");

        for (const std::string& groupName : groupNames) {
            bool foundGroup {false};

            // Check for an existing group we can fit in
            for (int i = 0; i < result.groupCount; ++i) {
                ShortcutGroup& group = result.groups[i];

                if (group.name == groupName) {
                    group.members[group.memberCount] = &shortcut;
                    group.memberCount++;
                    shortcut.groups[shortcut.groupCount] = &group;
                    shortcut.groupCount++;
                    foundGroup = true;
                }
            }

            // We are the first member of the group, so create a new one
            if (!foundGroup) {
                ShortcutGroup& newGroup = result.groups[result.groupCount];
                newGroup.name = groupName;
                newGroup.members[0] = &shortcut;
                newGroup.memberCount = 1;
                result.groupCount++;
                shortcut.groups[shortcut.groupCount] = &newGroup;
                shortcut.groupCount++;
            }
        }
    }

    return result;
}

void ShortcutGui::draw() {
    for (int i = 0; i < groupCount; ++i) {
        ShortcutGroup& group = groups[i];

        if (ImGui::CollapsingHeader(group.name.c_str())) {
            for (int j = 0; j < group.memberCount; ++j) {
                Shortcut* shortcut = group.members[j];

                if (ImGui::Button(shortcut->path.string().c_str())) {
                    Platform::openPath(shortcut->path);
                }
                ImGui::SameLine();
                ImGui::Text("%s", shortcut->description.c_str());
            }
        }
    }
}
