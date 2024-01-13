#ifndef POKEDEV_PLATFORM_HPP
#define POKEDEV_PLATFORM_HPP

#ifdef _WIN32
#include <filesystem>
#else
#include <vector>
#include <filesystem>
#endif

#include <optional>
#include <SDL.h>

/// The tools that handle platform-specific tasks, and ensures compatability on all devices.
/// This includes windows, file systems, rendering, ect.
/// It uses the SDL library whenever possible.
namespace Platform
{
    void init();
    void shutdown();
    void startFrame();
    void endFrame();

    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    bool wantsToQuit();

    // File System
    struct FilePickerOptions
    {
        std::optional<std::filesystem::path> defaultPath {};
        std::optional<std::string_view> filter {};
    };

    // todo: try save file as?
    bool tryPickFile(std::filesystem::path& outPath, const FilePickerOptions& options);
    bool tryPickFolder(std::filesystem::path& outPath, const FilePickerOptions& options);
    void openPath(const std::filesystem::path& path);
    void openFile(const std::filesystem::path& path);

} // namespace Platform

#endif // POKEDEV_PLATFORM_HPP