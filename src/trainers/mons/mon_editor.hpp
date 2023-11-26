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
    GameData* m_gameData {};
    TrainerMonData* m_dataToEdit {};
    ImGuiUtils::FilteredCombo m_abilityCombo {};
    ImGuiUtils::FilteredCombo m_itemCombo {};
    ImGuiUtils::FilteredCombo m_move1Combo {};
    ImGuiUtils::FilteredCombo m_move2Combo {};
    ImGuiUtils::FilteredCombo m_move3Combo {};
    ImGuiUtils::FilteredCombo m_move4Combo {};
    ImGuiUtils::FilteredCombo m_natureCombo {};
    ImGuiUtils::FilteredCombo m_ballCombo {};
    ImGuiUtils::FilteredCombo m_speciesCombo {};

};
