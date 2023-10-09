#include <imgui.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include <SDL.h>
#include <string>
#include "file_dialog.hpp"
#include "imgui_utils.hpp"

using namespace std;

namespace ImGuiUtils
{
    void FolderPicker(const char* label, filesystem::path& path, std::filesystem::path* defaultPath)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(format("View##{}", label).c_str()))
            system(format("explorer.exe {}", path.string()).c_str()); // todo: this only works on windows

        ImGui::SameLine();

        if (ImGui::Button(format("Edit##{}", label).c_str()))
            FileDialog::tryPickFolder(path, defaultPath != nullptr ? defaultPath->string().c_str() : nullptr);

        ImGui::SameLine();
        string pathString {path.string()};

        if (ImGui::InputTextWithHint(format("##{}", label).c_str(), "No folder selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

    void FilePicker(const char* label, filesystem::path& path, std::filesystem::path* defaultPath, const char* filter)
    {
        ImGui::Text("%s", label);

        if (ImGui::Button(format("View##{}", label).c_str()))
            system(format("explorer.exe {}", path.string()).c_str());

        ImGui::SameLine();

        if (ImGui::Button(format("Edit##{}", label).c_str()))
            FileDialog::tryPickFile(path, filter, defaultPath != nullptr ? defaultPath->string().c_str() : nullptr);

        ImGui::SameLine();
        string pathString {path.string()};

        if (ImGui::InputTextWithHint(format("##{}", label).c_str(), "No file selected", &pathString))
            path = filesystem::path {pathString};

        ImGui::Spacing(); ImGui::Spacing();
    }

} // namespace ImGuiUtils