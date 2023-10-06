#include <imgui.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include <fmt/format.h>
#include <SDL.h>
#include <string>
#include "file_dialog.hpp"
#include "imgui_utils.hpp"

using namespace std;

namespace ImGuiUtils
{
    void FolderPicker(const char* label, filesystem::path& path)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(fmt::format("Edit##{}", label).c_str()))
            FileDialog::tryPickFolder(path);

        ImGui::SameLine();

        if (ImGui::Button(fmt::format("Open##{}", label).c_str()))
            system(fmt::format("explorer.exe {}", path.string()).c_str()); // todo: this only works on windows

        ImGui::SameLine();
        string pathString {path.string()};
        ImGui::SetNextItemWidth(500);

        if (ImGui::InputTextWithHint(fmt::format("##{}", label).c_str(), "No folder selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

    void FilePicker(const char* label, filesystem::path& path, const char* filter)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(fmt::format("Edit##{}", label).c_str()))
            FileDialog::tryPickFile(path, filter);

        ImGui::SameLine();

        if (ImGui::Button(fmt::format("Open##{}", label).c_str()))
            system(fmt::format("explorer.exe {}", path.string()).c_str());

        ImGui::SameLine();
        string pathString {path.string()};
        ImGui::SetNextItemWidth(500);

        if (ImGui::InputTextWithHint(fmt::format("##{}", label).c_str(), "No file selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

} // namespace ImGuiUtils