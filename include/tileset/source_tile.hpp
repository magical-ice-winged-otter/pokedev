#ifndef POKEDEV_SOURCE_TILE_HPP
#define POKEDEV_SOURCE_TILE_HPP

#include <imgui.h>
#include <SDL.h>
#include <string>
#include <filesystem>
#include "event.hpp"

namespace PokeDev {
    class SourceTile {
    public:
        Event onChange;
        SourceTile();
        void drawEditor();
        SDL_Surface* getSurface();
        SDL_Texture* getTexture();
        [[nodiscard]] const SDL_Surface* getSurface() const;
        [[nodiscard]] const SDL_Texture* getTexture() const;
        [[nodiscard]] const std::string& getName() const;
        [[nodiscard]] const std::filesystem::path& getPath() const;

    private:
        SDL_Surface* m_surface;
        SDL_Texture* m_texture;
        std::filesystem::path m_imagePath;
        std::string m_name;

        void setSurface(const char* file);
    };

    namespace ImGuiUtil {
        ImVec2 getSize(const SourceTile& tile);
    }
}

#endif // POKEDEV_SOURCE_TILE_HPP