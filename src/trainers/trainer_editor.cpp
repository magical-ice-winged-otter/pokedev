#include "trainer_editor.hpp"
#include "application.hpp"

void TrainerEditor::init() {
    m_monEditor.init();
    setDataToEdit(nullptr);
}

void TrainerEditor::setDataToEdit(TrainerData *data) {
    m_data = data;

    if (data != nullptr) {
        if (data->partyMemberCount > 0) {
            m_monEditor.setDataToEdit(&data->partyMembers[0]);
            m_editedPartyMemberIndex = 0;
        }
    }
}

void TrainerEditor::draw() {
    if (ImGui::BeginListBox("Party Members")) {
        for (int i = 0; i < m_data->partyMemberCount; i++) {
            ImGui::PushID(i);
            const char* speciesName = Application::loaders.species.names[m_data->partyMembers[i].speciesIndex].c_str();
            if (ImGui::Selectable(speciesName, m_editedPartyMemberIndex == i)) {
                m_editedPartyMemberIndex = i;
                m_monEditor.setDataToEdit(&m_data->partyMembers[i]);
            }
            if (m_editedPartyMemberIndex == i) {
                ImGui::SetItemDefaultFocus();
            }
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }
    ImGui::InputInt("Party Size", &m_data->partyMemberCount);
    m_data->partyMemberCount = std::clamp(m_data->partyMemberCount, 1, 6);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Mon Editor", ImVec2(0, 350), true);
    m_monEditor.draw();
    ImGui::EndChild();
    ImGui::PopStyleVar();
}
