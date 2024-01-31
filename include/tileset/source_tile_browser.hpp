#ifndef POKEDEV_SOURCE_TILE_BROWSER_HPP
#define POKEDEV_SOURCE_TILE_BROWSER_HPP

#include <memory>
#include <vector>
#include "imgui_window.hpp"
#include "tileset/source_tile_editor.hpp"
#include "tileset/source_tile.hpp"

namespace PokeDev {
    typedef std::vector<std::shared_ptr<SourceTile>> SourceTileCollection;

    class SourceTileBrowser : public PokeDevWindow
    {
    public:
        SourceTileBrowser(std::shared_ptr<SourceTileCollection> tileCollection);
        const char *getName() override;
        void renderWindow() override;

    private:
        std::vector<SourceTileEditor> m_activeEditors;
        std::shared_ptr<SourceTileCollection> m_tileCollection;
    };
}

#endif // POKEDEV_SOURCE_TILE_BROWSER_HPP