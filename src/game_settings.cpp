#include "game_settings.hpp"
#include "imgui_utils.hpp"

void GameSettings::draw() {
    ImGuiUtils::folderPicker("Project Path", projectPath, {});
}
