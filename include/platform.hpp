#ifndef POKETOOLS_PLATFORM_HPP
#define POKETOOLS_PLATFORM_HPP

#include <filesystem>
#include <SDL.h>

namespace Platform
{
    // Lifetime
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

} // namespace Platform

#endif // POKETOOLS_PLATFORM_HPP