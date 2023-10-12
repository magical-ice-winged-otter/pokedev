#ifndef POKETOOLS_PLATFORM_HPP
#define POKETOOLS_PLATFORM_HPP

#include <filesystem>

namespace Platform
{
    void init();
    void shutdown();

    struct FilePickerOptions
    {
        std::optional<std::filesystem::path> defaultPath {};
        std::optional<std::string_view> filter {};
    };

    bool tryPickFile(std::filesystem::path& outPath, const FilePickerOptions& options);
    bool tryPickFolder(std::filesystem::path& outPath, const FilePickerOptions& options);
    void openPath(const std::filesystem::path& path);

} // namespace Platform

#endif // POKETOOLS_PLATFORM_HPP