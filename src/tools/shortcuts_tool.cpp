#include <fstream>
#include <string>
#include <imgui.h>
#include "tools/shortcuts_tool.hpp"
#include "string_parsing_util.hpp"
#include "platform.hpp"
#include "application.hpp"
#include "imgui_utils.hpp"

ShortcutsTool::ShortcutsTool() {
    name = "Shortcuts";
    init();
}

void ShortcutsTool::init() {
    std::ifstream shortcutFileStream {shortcutFilePath};

    if (!shortcutFileStream.is_open()) {
        return;
    }

    std::string line {};
    groupCount = 0;
    shortcutCount = 0;

    while (std::getline(shortcutFileStream, line)) {
        if (line.empty()) {
            continue;
        }

        std::string rawPath;
        std::string rawDescription;
        std::string rawGroups;

        tryGetCsvColumn(line, 0, rawPath);
        tryGetCsvColumn(line, 1, rawDescription);
        tryGetCsvColumn(line, 2, rawGroups);

        Shortcut& shortcut = shortcuts[shortcutCount];
        shortcut.path = rawPath;
        shortcut.description = getBetween(rawDescription, '"');
        shortcutCount++;

        // Now, extract the raw group names from the final column and parse them.
        std::vector<std::string> groupNames = split(rawGroups, "|");

        for (const std::string& groupName : groupNames) {
            bool foundGroup {false};

            // Check for an existing group we can fit in
            for (int i = 0; i < groupCount; ++i) {
                ShortcutGroup& group = groups[i];

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
                ShortcutGroup& newGroup = groups[groupCount];
                newGroup.name = groupName;
                newGroup.members[0] = &shortcut;
                newGroup.memberCount = 1;
                groupCount++;
                shortcut.groups[shortcut.groupCount] = &newGroup;
                shortcut.groupCount++;
            }
        }
    }
}

void ShortcutsTool::renderWindow() {
    if (ImGui::Begin("Shortcuts", &isActive)) {
        ImGuiUtils::filePicker("Shortcut File", shortcutFilePath, {});

        if (ImGui::Button("Reload")) {
            init();
        }

        for (int i = 0; i < groupCount; ++i) {
            ShortcutGroup& group = groups[i];
            ImGui::PushID(i);

            if (ImGui::CollapsingHeader(group.name.c_str())) {
                if (ImGui::Button("Open All")) {
                    for (int j = 0; j < group.memberCount; ++j) {
                        Platform::openFile(Application::settings.projectPath / group.members[j]->path);
                    }
                }
                for (int j = 0; j < group.memberCount; ++j) {
                    Shortcut* shortcut = group.members[j];

                    if (ImGui::Button(shortcut->path.string().c_str())) {
                        Platform::openFile(Application::settings.projectPath / shortcut->path);
                    }
                    ImGui::SameLine();
                    ImGui::Text("%s", shortcut->description.c_str());
                }
            }
            ImGui::PopID();
        }
        ImGui::End();
    }
}
