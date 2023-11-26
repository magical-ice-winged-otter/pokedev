#include "trainer_editor.hpp"

void TrainerEditor::init(GameData &gameData) {
    m_gameData = &gameData;
    m_monEditor.init(gameData);
    setDataToEdit(nullptr);
}

void TrainerEditor::setDataToEdit(TrainerData *data) {
    m_dataToEdit = data;

    if (data != nullptr) {
        if (data->partyMemberCount > 0) {
            m_monEditor.setDataToEdit(&data->partyMembers[0]);
            m_editedPartyMemberIndex = 0;
        }
    }
}

void TrainerEditor::draw() {
    if (ImGui::BeginListBox("Party Members")) {
        for (int i = 0; i < m_dataToEdit->partyMemberCount; i++) {
            ImGui::PushID(i);
            const char* speciesName = m_gameData->speciesLoader.speciesNames[m_dataToEdit->partyMembers[i].speciesIndex].c_str();
            if (ImGui::Selectable(speciesName, m_editedPartyMemberIndex == i)) {
                m_editedPartyMemberIndex = i;
                m_monEditor.setDataToEdit(&m_dataToEdit->partyMembers[i]);
            }
            if (m_editedPartyMemberIndex == i) {
                ImGui::SetItemDefaultFocus();
            }
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }
    ImGui::InputInt("Party Size", &m_dataToEdit->partyMemberCount);
    m_dataToEdit->partyMemberCount = std::clamp(m_dataToEdit->partyMemberCount, 1, 6);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Mon Editor", ImVec2(0, 350), true);
    m_monEditor.draw();
    ImGui::EndChild();
    ImGui::PopStyleVar();
}
