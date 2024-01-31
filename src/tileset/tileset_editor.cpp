#include <memory>
#include "tileset/tileset.hpp"
#include "tileset/tileset_editor.hpp"

using namespace PokeDev;

TilesetEditor::TilesetEditor(std::shared_ptr<Tileset> activeTileset)
    : m_tileset{activeTileset}
{}

const char* TilesetEditor::getName() {
    return "Tileset Editor";
}

void TilesetEditor::renderWindow() {
    m_tileset->drawEditor();
}