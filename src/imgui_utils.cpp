#include <imgui.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include <SDL.h>
#include <string>
#include "imgui_utils.hpp"
#include "platform.hpp"

using namespace std;

namespace ImGuiUtils
{
    void FolderPicker(const char* label, filesystem::path& path, const Platform::FilePickerOptions& options)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(format("View##{}", label).c_str()))
            Platform::openPath(path);

        ImGui::SameLine();

        if (ImGui::Button(format("Edit##{}", label).c_str()))
            Platform::tryPickFolder(path, options);

        ImGui::SameLine();
        string pathString {path.string()};

        if (ImGui::InputTextWithHint(format("##{}", label).c_str(), "No folder selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

    void FilePicker(const char* label, filesystem::path& path, const Platform::FilePickerOptions& options)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(format("View##{}", label).c_str()))
            system(format("explorer.exe {}", path.string()).c_str());

        ImGui::SameLine();

        if (ImGui::Button(format("Edit##{}", label).c_str()))
            Platform::tryPickFile(path, options);

        ImGui::SameLine();
        string pathString {path.string()};

        if (ImGui::InputTextWithHint(format("##{}", label).c_str(), "No file selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

} // namespace ImGuiUtils