#ifndef POKEDEV_SOURCE_TILE_HPP
#define POKEDEV_SOURCE_TILE_HPP

#include <SDL.h>
#include <string>
#include <filesystem>

namespace PokeDev {
    class SourceTile {
    public:
        SourceTile();
        void drawEditor();
        SDL_Surface* getSurface();
        SDL_Texture* getTexture();
        const std::string& getName();
        const std::filesystem::path& getPath();

    private:
        SDL_Surface* m_surface;
        SDL_Texture* m_texture;
        std::filesystem::path m_imagePath;
        std::string m_name;

        void setSurface(const char* file);
    };
}

#endif // POKEDEV_SOURCE_TILE_HPP