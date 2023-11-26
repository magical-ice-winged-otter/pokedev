#include "mon_editor.hpp"
#include "game_data.hpp"
#include <imgui.h>

void MonEditor::init(GameData& gameData)
{
    m_gameData = &gameData;
    m_abilityCombo.init(&m_gameData->abilityLoader.abilityNames, "Abilities");
    m_itemCombo.init(&m_gameData->itemLoader.itemNames, "Items");
    m_move1Combo.init(&m_gameData->moveLoader.moveNames, "##Move 1");
    m_move2Combo.init(&m_gameData->moveLoader.moveNames, "##Move 2");
    m_move3Combo.init(&m_gameData->moveLoader.moveNames, "##Move 3");
    m_move4Combo.init(&m_gameData->moveLoader.moveNames, "##Move 4");
    m_natureCombo.init(&m_gameData->natureLoader.natureNames, "Nature");
    m_ballCombo.init(&m_gameData->ballLoader.ballNames, "Ball");
    m_speciesCombo.init(&m_gameData->speciesLoader.speciesNames, "Species");
    setDataToEdit(nullptr);
}

void MonEditor::setDataToEdit(TrainerMonData *data)
{
    m_dataToEdit = data;
    m_abilityCombo.selectedIndex = data != nullptr ? data->abilityIndex : 0;
    m_itemCombo.selectedIndex = data != nullptr ? data->itemIndex : 0;
    m_move1Combo.selectedIndex = data != nullptr ? data->movesIndex[0] : 0;
    m_move2Combo.selectedIndex = data != nullptr ? data->movesIndex[1] : 0;
    m_move3Combo.selectedIndex = data != nullptr ? data->movesIndex[2] : 0;
    m_move4Combo.selectedIndex = data != nullptr ? data->movesIndex[3] : 0;
    m_natureCombo.selectedIndex = data != nullptr ? data->natureIndex : 0;
    m_ballCombo.selectedIndex = data != nullptr ? data->ballIndex : 0;
    m_speciesCombo.selectedIndex = data != nullptr ? data->speciesIndex : 0;
}

void MonEditor::draw()
{
    if (ImGui::CollapsingHeader("Output"))
    {
        ImGui::Text("%s", generateMonStruct(*m_dataToEdit, *m_gameData).c_str());
    }

    // Species Combo
    {
        m_speciesCombo.draw();
        m_dataToEdit->speciesIndex = m_speciesCombo.selectedIndex;
    }

    // Ability Combo
    {
        ImGui::Checkbox("##hasAbility", &m_dataToEdit->hasAbility);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasAbility);
        m_abilityCombo.draw();
        m_dataToEdit->abilityIndex = m_abilityCombo.selectedIndex;
        ImGui::EndDisabled();
    }
    // Item Combo
    {
        ImGui::Checkbox("##hasItem", &m_dataToEdit->hasItem);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasItem);
        m_itemCombo.draw();
        m_dataToEdit->itemIndex = m_itemCombo.selectedIndex;
        ImGui::EndDisabled();
    }
    // Moves Combos
    {
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 4);
        ImGui::Checkbox("##hasMoves", &m_dataToEdit->hasMoves);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasMoves);

        m_move1Combo.draw();
        m_dataToEdit->movesIndex[0] = m_move1Combo.selectedIndex;

        ImGui::SameLine();
        m_move2Combo.draw();
        m_dataToEdit->movesIndex[1] = m_move2Combo.selectedIndex;

        ImGui::SameLine();
        m_move3Combo.draw();
        m_dataToEdit->movesIndex[2] = m_move3Combo.selectedIndex;

        ImGui::SameLine();
        m_move4Combo.draw();
        m_dataToEdit->movesIndex[3] = m_move4Combo.selectedIndex;

        ImGui::EndDisabled();
        ImGui::PopItemWidth();
    }
    // Nature Combo
    {
        ImGui::Checkbox("##hasNature", &m_dataToEdit->hasNature);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasNature);
        m_natureCombo.draw();
        m_dataToEdit->natureIndex = m_natureCombo.selectedIndex;
        ImGui::EndDisabled();
    }
    // Ball Combo
    {
        ImGui::Checkbox("##hasBall", &m_dataToEdit->hasBall);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasBall);
        m_ballCombo.draw();
        m_dataToEdit->ballIndex = m_ballCombo.selectedIndex;
        ImGui::EndDisabled();
    }
}