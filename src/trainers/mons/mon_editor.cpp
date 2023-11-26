#include <imgui.h>
#include <imgui_stdlib.h>
#include "mon_editor.hpp"
#include "game_data.hpp"

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

void MonEditor::setDataToEdit(MonData *data)
{
    m_dataToEdit = data;

    if (data != nullptr) {
        m_abilityCombo.selectedIndex = data->abilityIndex;
        m_itemCombo.selectedIndex = data->itemIndex;
        m_move1Combo.selectedIndex = data->movesIndex[0];
        m_move2Combo.selectedIndex = data->movesIndex[1];
        m_move3Combo.selectedIndex = data->movesIndex[2];
        m_move4Combo.selectedIndex = data->movesIndex[3];
        m_natureCombo.selectedIndex = data->natureIndex;
        m_ballCombo.selectedIndex = data->ballIndex;
        m_speciesCombo.selectedIndex = data->speciesIndex;
    }
}

void MonEditor::draw()
{
    if (m_dataToEdit == nullptr) {
        return;
    }

    // Species Combo
    {
        m_speciesCombo.draw();
        m_dataToEdit->speciesIndex = m_speciesCombo.selectedIndex;
    }

    // Level
    ImGui::InputInt("Level", &m_dataToEdit->level);
    m_dataToEdit->level = std::clamp(m_dataToEdit->level, 0, 100);

    // Gender
    {
        ImGui::Checkbox("##hasGender", &m_dataToEdit->hasGender);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasGender);

        if (ImGui::RadioButton("Male", m_dataToEdit->gender == MonGender::Male)) {
            m_dataToEdit->gender = MonGender::Male;
        }

        ImGui::SameLine();

        if (ImGui::RadioButton("Female", m_dataToEdit->gender == MonGender::Female)) {
            m_dataToEdit->gender = MonGender::Female;
        }

        ImGui::EndDisabled();
    }

    // Nickname
    {
        ImGui::Checkbox("##hasNickname", &m_dataToEdit->hasNickname);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasNickname);
        ImGui::InputText("Nickname", &m_dataToEdit->nickname);
        ImGui::EndDisabled();
    }

    // Is Shiny
    {
        ImGui::Checkbox("##hasIsShiny", &m_dataToEdit->hasIsShiny);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasIsShiny);
        ImGui::Checkbox("Is Shiny", &m_dataToEdit->isShiny);
        ImGui::EndDisabled();
    }

    // EVs
    {
        ImGui::Checkbox("##hasEvs", &m_dataToEdit->hasEvs);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasEvs);
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 8);

        if (ImGui::Button("Max##ev")) {
            for (int i = 0; i < 6; i++) {
                m_dataToEdit->evs[i] = 252;
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Zero##ev")) {
            for (int i = 0; i < 6; i++) {
                m_dataToEdit->evs[i] = 0;
            }
        }
        ImGui::SameLine();

        for (int i = 0; i < 6; i++) {
            ImGui::PushID(i);
            ImGui::DragInt("##EV", &m_dataToEdit->evs[i], 1, 0, 252);

            if (i < 5) {
                ImGui::SameLine();
            }

            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndDisabled();
    }

    // IVs
    {
        ImGui::Checkbox("##hasIvs", &m_dataToEdit->hasIvs);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasIvs);
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 8);

        if (ImGui::Button("Max##iv")) {
            for (int i = 0; i < 6; i++) {
                m_dataToEdit->ivs[i] = 252;
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Zero##iv")) {
            for (int i = 0; i < 6; i++) {
                m_dataToEdit->ivs[i] = 0;
            }
        }
        ImGui::SameLine();

        for (int i = 0; i < 6; i++) {
            ImGui::PushID(i);
            ImGui::DragInt("##IV", &m_dataToEdit->ivs[i], 1, 0, 252);

            if (i < 5) {
                ImGui::SameLine();
            }

            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndDisabled();
    }

    // Friendship
    {
        ImGui::Checkbox("##hasFriendship", &m_dataToEdit->hasFriendship);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_dataToEdit->hasFriendship);

        ImGui::InputInt("Friendship", &m_dataToEdit->friendship);
        m_dataToEdit->friendship = std::clamp(m_dataToEdit->friendship, 0, 255);

        ImGui::EndDisabled();
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

    if (ImGui::CollapsingHeader("Output")) {
        ImGui::Text("%s", generateMonStruct(*m_dataToEdit, *m_gameData).c_str());
    }
}