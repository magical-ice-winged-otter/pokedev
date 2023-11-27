#include <imgui.h>
#include <imgui_stdlib.h>
#include "mon_editor.hpp"
#include "game_loaders.hpp"
#include "application.hpp"

using namespace Application;

void MonEditor::init()
{
    m_abilityCombo.init(&loaders.abilities.names, "Abilities");
    m_itemCombo.init(&loaders.items.names, "Items");
    m_move1Combo.init(&loaders.moves.names, "##Move 1");
    m_move2Combo.init(&loaders.moves.names, "##Move 2");
    m_move3Combo.init(&loaders.moves.names, "##Move 3");
    m_move4Combo.init(&loaders.moves.names, "##Move 4");
    m_natureCombo.init(&loaders.natures.names, "Nature");
    m_ballCombo.init(&loaders.balls.names, "Ball");
    m_speciesCombo.init(&loaders.species.names, "Species");
    setDataToEdit(nullptr);
}

void MonEditor::setDataToEdit(MonData *data)
{
    m_data = data;

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
    if (m_data == nullptr) {
        return;
    }

    // Species Combo
    {
        m_speciesCombo.draw();
        m_data->speciesIndex = m_speciesCombo.selectedIndex;
    }

    // Level
    ImGui::InputInt("Level", &m_data->level);
    m_data->level = std::clamp(m_data->level, 0, 100);

    // Gender
    {
        ImGui::Checkbox("##hasGender", &m_data->hasGender);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasGender);

        if (ImGui::RadioButton("Male", m_data->gender == MonGender::Male)) {
            m_data->gender = MonGender::Male;
        }

        ImGui::SameLine();

        if (ImGui::RadioButton("Female", m_data->gender == MonGender::Female)) {
            m_data->gender = MonGender::Female;
        }

        ImGui::EndDisabled();
    }

    // Nickname
    {
        ImGui::Checkbox("##hasNickname", &m_data->hasNickname);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasNickname);
        ImGui::InputText("Nickname", &m_data->nickname);
        ImGui::EndDisabled();
    }

    // Is Shiny
    {
        ImGui::Checkbox("##hasIsShiny", &m_data->hasIsShiny);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasIsShiny);
        ImGui::Checkbox("Is Shiny", &m_data->isShiny);
        ImGui::EndDisabled();
    }

    // EVs
    {
        ImGui::Checkbox("##hasEvs", &m_data->hasEvs);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasEvs);
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 8);

        if (ImGui::Button("Max##ev")) {
            for (int i = 0; i < 6; i++) {
                m_data->evs[i] = 252;
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Zero##ev")) {
            for (int i = 0; i < 6; i++) {
                m_data->evs[i] = 0;
            }
        }
        ImGui::SameLine();

        for (int i = 0; i < 6; i++) {
            ImGui::PushID(i);
            ImGui::DragInt("##EV", &m_data->evs[i], 1, 0, 252);

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
        ImGui::Checkbox("##hasIvs", &m_data->hasIvs);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasIvs);
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 8);

        if (ImGui::Button("Max##iv")) {
            for (int i = 0; i < 6; i++) {
                m_data->ivs[i] = 252;
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Zero##iv")) {
            for (int i = 0; i < 6; i++) {
                m_data->ivs[i] = 0;
            }
        }
        ImGui::SameLine();

        for (int i = 0; i < 6; i++) {
            ImGui::PushID(i);
            ImGui::DragInt("##IV", &m_data->ivs[i], 1, 0, 252);

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
        ImGui::Checkbox("##hasFriendship", &m_data->hasFriendship);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasFriendship);

        ImGui::InputInt("Friendship", &m_data->friendship);
        m_data->friendship = std::clamp(m_data->friendship, 0, 255);

        ImGui::EndDisabled();
    }

    // Ability Combo
    {
        ImGui::Checkbox("##hasAbility", &m_data->hasAbility);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasAbility);
        m_abilityCombo.draw();
        m_data->abilityIndex = m_abilityCombo.selectedIndex;
        ImGui::EndDisabled();
    }

    // Item Combo
    {
        ImGui::Checkbox("##hasItem", &m_data->hasItem);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasItem);
        m_itemCombo.draw();
        m_data->itemIndex = m_itemCombo.selectedIndex;
        ImGui::EndDisabled();
    }

    // Moves Combos
    {
        ImGui::PushItemWidth(ImGui::CalcItemWidth() / 4);
        ImGui::Checkbox("##hasMoves", &m_data->hasMoves);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasMoves);

        m_move1Combo.draw();
        m_data->movesIndex[0] = m_move1Combo.selectedIndex;

        ImGui::SameLine();
        m_move2Combo.draw();
        m_data->movesIndex[1] = m_move2Combo.selectedIndex;

        ImGui::SameLine();
        m_move3Combo.draw();
        m_data->movesIndex[2] = m_move3Combo.selectedIndex;

        ImGui::SameLine();
        m_move4Combo.draw();
        m_data->movesIndex[3] = m_move4Combo.selectedIndex;

        ImGui::EndDisabled();
        ImGui::PopItemWidth();
    }

    // Nature Combo
    {
        ImGui::Checkbox("##hasNature", &m_data->hasNature);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasNature);
        m_natureCombo.draw();
        m_data->natureIndex = m_natureCombo.selectedIndex;
        ImGui::EndDisabled();
    }

    // Ball Combo
    {
        ImGui::Checkbox("##hasBall", &m_data->hasBall);
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_data->hasBall);
        m_ballCombo.draw();
        m_data->ballIndex = m_ballCombo.selectedIndex;
        ImGui::EndDisabled();
    }

    if (ImGui::CollapsingHeader("Output")) {
        ImGui::Text("%s", m_data->generateStruct().c_str());
    }
}