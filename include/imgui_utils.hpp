#ifndef POKEDEV_IMGUI_UTILS_HPP
#define POKEDEV_IMGUI_UTILS_HPP

#include <filesystem>
#include "platform.hpp"

/// Re-usable, custom ImGui widgets.
namespace ImGuiUtils
{
    void FolderPicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);
    void FilePicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);

} // namespace ImGuiUtils

#endif // POKEDEV_IMGUI_UTILS_HPP