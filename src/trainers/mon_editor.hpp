#pragma once

#include "mon_data.hpp"
#include "imgui_utils.hpp"
#include "game_data.hpp"

class MonEditor
{
public:
    void init(GameData& gameData);
    void setDataToEdit(TrainerMonData* data);
    void draw();

private:
    GameData* m_gameData;
    TrainerMonData* m_dataToEdit;
    ImGuiUtils::FilteredCombo m_abilityCombo;
};
