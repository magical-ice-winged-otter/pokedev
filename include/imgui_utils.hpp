#ifndef PORYTILES_GUI_IMGUI_UTILS_HPP
#define PORYTILES_GUI_IMGUI_UTILS_HPP

#include <filesystem>

namespace ImGuiUtils
{
    void FolderPicker(const char* label, std::filesystem::path& path);
    void FilePicker(const char* label, std::filesystem::path& path, const char* filter = nullptr);

} // namespace ImGuiUtils

#endif // PORYTILES_GUI_IMGUI_UTILS_HPP