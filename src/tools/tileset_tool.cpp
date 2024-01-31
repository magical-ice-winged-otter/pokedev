#include "tools/tileset_tool.hpp"
#include "tileset/source_tile.hpp"
#include "tileset/tileset.hpp"

using namespace PokeDev;

TilesetTool::TilesetTool()
    : m_tileCollection{new SourceTileCollection{}}, m_tilesetCollection{new TilesetCollection{}}, tilesetBrowser{m_tilesetCollection}, tileCollectionBrowser{m_tileCollection}
{
    // todo: load source tiles?
    // todo: load tilesets?
}

void TilesetTool::renderWindow() {
    tilesetBrowser.renderAlwaysOpen();
    tileCollectionBrowser.renderAlwaysOpen();
}

const char* TilesetTool::getName() {
    return "Tileset Tool";
}