#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>
#include "imgui_utils.hpp"
#include "platform.hpp"

using namespace std;

namespace ImGuiUtils
{
    void FilteredCombo::init(const vector<std::string>* values, const char* label)
    {
        m_wasOpen = false;
        m_hasFilter = false;
        m_filter = ImGuiTextFilter {};
        m_values = values;
        m_passedValueIndices.clear();
        m_label = label;
    }

    bool FilteredCombo::draw()
    {
        if (m_values->empty())
            return false;

        bool isOpen = ImGui::BeginCombo(m_label, (*m_values)[selectedIndex].c_str());

        if (isOpen)
        {
            // Keep track of the open history, so we can autofocus on the filter input.
            if (!m_wasOpen)
                ImGui::SetKeyboardFocusHere();

            m_filter.Draw();

            // If the filter is edited, compute a list of all indices that pass the filter.
            if (ImGui::IsItemEdited())
            {
                m_hasFilter = true;
                updatePassedValueIndices();
            }

            if (m_hasFilter) // Display all the non-filtered items.
            {
                ImGuiListClipper clipper;
                clipper.Begin(static_cast<int>(m_passedValueIndices.size()));

                while (clipper.Step())
                {
                    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    {
                        if (ImGui::Selectable((*m_values)[m_passedValueIndices[i]].c_str()))
                            selectedIndex = m_passedValueIndices[i];
                    }
                }
            }
            else // We don't have a filter, so just display all the items.
            {
                ImGuiListClipper clipper;
                clipper.Begin(static_cast<int>(m_values->size()));

                while (clipper.Step())
                {
                    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    {
                        if (ImGui::Selectable((*m_values)[i].c_str()))
                            selectedIndex = i;
                    }
                }
            }

            ImGui::EndCombo();
        }

        bool justClosed = m_wasOpen && !isOpen;

        if (justClosed)
        {
            m_filter.Clear();
            m_hasFilter = false;
        }

        m_wasOpen = isOpen;
        return justClosed;
    }

    void FilteredCombo::updatePassedValueIndices()
    {
        m_passedValueIndices.clear();

        for (size_t i = 0; i < m_values->size(); i++)
        {
            if (m_filter.PassFilter((*m_values)[i].c_str()))
                m_passedValueIndices.push_back(i);
        }
    }

    void folderPicker(const char* label, filesystem::path& path, const Platform::FilePickerOptions& options)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(format("View##{}", label).c_str()))
            Platform::openPath(path);

        ImGui::SameLine();

        if (ImGui::Button(format("Edit##{}", label).c_str()))
            tryPickFolder(path, options);

        ImGui::SameLine();
        string pathString{path.string()};

        if (ImGui::InputTextWithHint(format("##{}", label).c_str(), "No folder selected", &pathString))
            path = filesystem::path{pathString};

        ImGui::Spacing();
        ImGui::Spacing();
    }

    void filePicker(const char* label, filesystem::path& path, const Platform::FilePickerOptions& options)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(format("Open##{}", label).c_str()))
            Platform::openFile(path);

        ImGui::SameLine();

        if (ImGui::Button(format("View##{}", label).c_str()))
            Platform::openPath(path.parent_path());

        ImGui::SameLine();


        if (ImGui::Button(format("Edit##{}", label).c_str()))
            tryPickFile(path, options);

        ImGui::SameLine();
        string pathString{path.string()};

        if (ImGui::InputTextWithHint(format("##{}", label).c_str(), "No file selected", &pathString))
            path = filesystem::path{pathString};

        ImGui::Spacing();
        ImGui::Spacing();
    }
} // namespace ImGuiUtils
