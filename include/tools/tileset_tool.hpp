#ifndef POKEDEV_TILESET_TOOL_HPP
#define POKEDEV_TILESET_TOOL_HPP

#include <memory>
#include "imgui_window.hpp"
#include "tileset/source_tile.hpp"
#include "tileset/tileset.hpp"
#include "tileset/tileset_browser.hpp"
#include "tileset/source_tile_browser.hpp"

namespace PokeDev {
    // todo: serialize
    class TilesetTool : public PokeDevWindow
    {
    public:
        TilesetTool();
        void renderWindow() override;
        const char *getName() override;

    private:
        std::shared_ptr<SourceTileCollection> m_tileCollection;
        std::shared_ptr<TilesetCollection> m_tilesetCollection;
        TilesetBrowser tilesetBrowser;
        SourceTileBrowser tileCollectionBrowser;
    };
}

#endif // POKEDEV_TILESET_TOOL_HPP