#ifndef POKEDEV_SOURCE_TILE_EDITOR_HPP
#define POKEDEV_SOURCE_TILE_EDITOR_HPP

#include <memory>
#include <string>
#include "imgui_window.hpp"
#include "tileset/source_tile.hpp"

namespace PokeDev {
    class SourceTileEditor : public PokeDevWindow
    {
    public:
        SourceTileEditor(std::shared_ptr<SourceTile> activeTile);
        const char *getName() override;
        void renderWindow() override;

    private:
        std::string m_name;
        std::shared_ptr<SourceTile> m_tile;
    };
}

#endif // POKEDEV_SOURCE_TILE_EDITOR_HPP