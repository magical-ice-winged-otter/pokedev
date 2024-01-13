#ifndef POKEDEV_IMGUI_UTILS_HPP
#define POKEDEV_IMGUI_UTILS_HPP

#ifdef _WIN32
#include <filesystem>
#else
#include <vector>
#include <filesystem>
#endif

#include <imgui.h>
#include <format>
#include "platform.hpp"

/// Re-usable, custom ImGui widgets.
namespace ImGuiUtils
{
    /**
     * \brief A combo box that can be filtered. It is optimized for displaying
     * and filtering a long list of items.
     */
    class FilteredCombo
    {
    public:
        void init(const std::vector<std::string>* values, const char* label);
        bool draw();
        size_t selectedIndex {};
    private:
        void updatePassedValueIndices();
        const std::vector<std::string>* m_values {};
        const char* m_label {};
        std::vector<size_t> m_passedValueIndices {};
        bool m_wasOpen {};
        bool m_hasFilter {};
        ImGuiTextFilter m_filter {};
    };

    void folderPicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);
    void filePicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);

} // namespace ImGuiUtils

#endif // POKEDEV_IMGUI_UTILS_HPP
