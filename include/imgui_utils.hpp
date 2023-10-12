#ifndef PORYTILES_GUI_IMGUI_UTILS_HPP
#define PORYTILES_GUI_IMGUI_UTILS_HPP

#include <filesystem>
#include "platform.hpp"

namespace ImGuiUtils
{
    void FolderPicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);
    void FilePicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);

} // namespace ImGuiUtils

#endif // PORYTILES_GUI_IMGUI_UTILS_HPP