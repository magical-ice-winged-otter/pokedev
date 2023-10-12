#ifndef POKETOOLS_PLATFORM_HPP
#define POKETOOLS_PLATFORM_HPP

#include <filesystem>
#include <SDL.h>

namespace Platform
{
    void init();
    void shutdown();
    void startFrame();
    void endFrame();

    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    bool wantsToQuit();

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