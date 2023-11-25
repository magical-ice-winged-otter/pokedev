#ifndef POKEDEV_IMGUI_UTILS_HPP
#define POKEDEV_IMGUI_UTILS_HPP

#include <filesystem>
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
        FilteredCombo(const std::vector<std::string>& values, const char* label, const char* filter = "");
        bool draw();
        size_t selectedIndex {};
    private:
        bool m_wasOpen {};
        bool m_hasFilter {};
        ImGuiTextFilter m_filter {};
        const std::vector<std::string>& m_values {};
        std::vector<size_t> m_passedValueIndices {};
        const char* m_label {};

        void updatePassedValueIndices();
    };

    void folderPicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);
    void filePicker(const char* label, std::filesystem::path& path, const Platform::FilePickerOptions& options);

} // namespace ImGuiUtils

#endif // POKEDEV_IMGUI_UTILS_HPP
