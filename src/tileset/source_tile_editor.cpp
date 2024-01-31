#include <filesystem>
#include <fmt/format.h>
#include <memory>
#include "tileset/source_tile.hpp"
#include "tileset/source_tile_editor.hpp"

using namespace PokeDev;

SourceTileEditor::SourceTileEditor(std::shared_ptr<SourceTile> activeTile)
    : m_tile{activeTile}, m_name{}
{
    m_name = fmt::format("\"{}\" Source Tile Editor", activeTile->getPath().string());
}

const char* SourceTileEditor::getName() {
    return m_name.c_str();
}

void SourceTileEditor::renderWindow() {
    m_tile->drawEditor();
}