#ifndef POKEDEV_TILESET_HPP
#define POKEDEV_TILESET_HPP

#include <imgui.h>
#include <vector>
#include <memory>
#include <SDL.h>
#include "source_tile.hpp"

namespace PokeDev {
    class Tile {
    public:
        explicit Tile(const std::shared_ptr<SourceTile>& sourceTile);
        ~Tile();
        void refresh();
        SDL_Surface* getSurface();
        SDL_Texture* getTexture();
        [[nodiscard]] const SDL_Surface* getSurface() const;
        [[nodiscard]] const SDL_Texture* getTexture() const;
    private:
        struct SourceTileInfo {
            std::shared_ptr<SourceTile> sourceTile;
            Event::ListenerHandle changeListener;
        };
        std::vector<SourceTileInfo> m_tileInfo;
        SDL_Surface* m_surface;
        SDL_Texture* m_texture;
        void addSourceTile(const std::shared_ptr<SourceTile>& sourceTile);
    };

    namespace ImGuiUtil {
        ImVec2 getSize(const Tile& tile);
    }

    class Tileset
    {
    public:
        Tileset();
        void drawEditor();
    private:
        std::vector<std::unique_ptr<Tile>> m_tiles;
        std::shared_ptr<SourceTile> m_defaultSourceTile;
        int m_width;
        int m_height;
    };

    typedef std::vector<std::shared_ptr<Tileset>> TilesetCollection;
}

#endif // POKEDEV_TILESET_HPP