#ifdef _WIN32
#ifndef POKEDEV_PLATFORM_WINDOWS_HPP
#define POKEDEV_PLATFORM_WINDOWS_HPP

#include "platform.hpp"

namespace Platform::Windows
{
    void init();
    void shutdown();
    bool tryPickFile(std::filesystem::path& outPath, const FilePickerOptions& options);
    bool tryPickFolder(std::filesystem::path& outPath, const FilePickerOptions& options);
    bool openPath(const std::filesystem::path& path);
    bool openFile(const std::filesystem::path& path);
}

#endif // POKEDEV_PLATFORM_WINDOWS_HPP
#endif // _WIN32