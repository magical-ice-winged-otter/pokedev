#include "mon_editor.hpp"
#include "game_data.hpp"
#include <imgui.h>

void MonEditor::init(GameData& gameData)
{
    m_gameData = &gameData;
    m_abilityCombo.init(&m_gameData->abilityLoader.abilityNames, "Abilities");
    setDataToEdit(nullptr);
}

void MonEditor::setDataToEdit(TrainerMonData *data)
{
    m_dataToEdit = data;
    m_abilityCombo.selectedIndex = data != nullptr ? data->abilityIndex : 0;
}

void MonEditor::draw()
{
    ImGui::Checkbox("##hasAbility", &m_dataToEdit->hasAbility);
    ImGui::SameLine();
    ImGui::BeginDisabled(!m_dataToEdit->hasAbility);
    m_abilityCombo.draw();
    ImGui::EndDisabled();
}