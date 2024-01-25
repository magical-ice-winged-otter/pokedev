#include <imgui.h>
#include <SDL_image.h>
#include "tileset/tileset.hpp"
#include "platform.hpp"

using namespace PokeDev;

ImVec2 ImGuiUtil::getSize(const Tile& tile) {
    return ImVec2 {static_cast<float>(tile.getSurface()->w), static_cast<float>(tile.getSurface()->h)};
}

Tileset::Tileset()
        : m_tiles {}
        , m_width {8}
        , m_height {3}
        , m_defaultSourceTile {}
{
    m_tiles.resize(m_width * m_height);
    m_defaultSourceTile = std::make_shared<SourceTile>();

    for (int i = 0; i < m_tiles.size(); ++i) {
        m_tiles[i] = std::make_unique<Tile>(m_defaultSourceTile);
    }
}

void Tileset::drawEditor() {
    ImGui::Text("Edit a tileset here!");

    for (int i = 0; i < m_tiles.size(); ++i) {
        if (i % m_width != 0) {
            ImGui::SameLine();
        }
        ImGui::Image(m_tiles[i]->getTexture(), ImGuiUtil::getSize(*m_tiles[i]));
    }
}

Tile::Tile(const std::shared_ptr<SourceTile>& sourceTile)
        : m_tileInfo {}
        , m_surface {}
        , m_texture {}
{
    addSourceTile(sourceTile);
}

Tile::~Tile() {
    for (int i = 0; i < m_tileInfo.size(); ++i) {
        SourceTileInfo& info = m_tileInfo[i];
        info.sourceTile->onChange.removeListener(info.changeListener);
    }
}

static void handleSourceTileChange(void* userData) {
    Tile* tile = static_cast<Tile*>(userData);
    tile->refresh();
}

void Tile::addSourceTile(const std::shared_ptr<SourceTile>& sourceTile) {
    SourceTileInfo info {
        .sourceTile = sourceTile,
        .changeListener = sourceTile->onChange.addListener(handleSourceTileChange, this),
    };
    m_tileInfo.push_back(info);
    refresh();
}

void Tile::refresh() {
    if (m_surface != nullptr) SDL_FreeSurface(m_surface);
    if (m_texture != nullptr) SDL_DestroyTexture(m_texture);

    if (m_tileInfo.empty()) {
        m_surface = IMG_Load("missing_tile.png");
        return;
    }
    else {
        m_surface = m_tileInfo[0].sourceTile->getSurface();

        if (m_tileInfo.size() > 1) {
            for (int i = 1; i < m_tileInfo.size(); ++i) {
                SDL_BlitSurface(m_tileInfo[i].sourceTile->getSurface(), nullptr, m_surface, nullptr);
            }
        }
    }

    m_texture = SDL_CreateTextureFromSurface(Platform::getRenderer(), m_surface);
}

const SDL_Surface *Tile::getSurface() const {
    return m_surface;
}

const SDL_Texture *Tile::getTexture() const {
    return m_texture;
}

SDL_Surface *Tile::getSurface() {
    return m_surface;
}

SDL_Texture *Tile::getTexture() {
    return m_texture;
}
