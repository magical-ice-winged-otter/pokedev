#pragma once

#include "mon_data.hpp"
#include "imgui_utils.hpp"
#include "game_loaders.hpp"

class MonEditor
{
public:
    void init();
    void setDataToEdit(MonData* data);
    void draw();

private:
    MonData* m_data {};
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
