#ifdef _WIN32
#ifndef POKETOOLS_PLATFORM_WINDOWS_HPP
#define POKETOOLS_PLATFORM_WINDOWS_HPP

#include "platform.hpp"

namespace Platform::Windows
{
    void init();
    void shutdown();
    bool tryPickFile(std::filesystem::path& outPath, const FilePickerOptions& options);
    bool tryPickFolder(std::filesystem::path& outPath, const FilePickerOptions& options);
    void openPath(const std::filesystem::path& path);
}

#endif // POKETOOLS_PLATFORM_WINDOWS_HPP
#endif // _WIN32