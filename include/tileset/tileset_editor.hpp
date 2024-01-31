#ifndef POKEDEV_TILESET_EDITOR_HPP
#define POKEDEV_TILESET_EDITOR_HPP

#include <memory>
#include "imgui_window.hpp"
#include "tileset/tileset.hpp"

namespace PokeDev {
    class TilesetEditor : public PokeDevWindow
    {
    public:
        TilesetEditor(std::shared_ptr<Tileset> activeTileset);
        const char *getName() override;
        void renderWindow() override;

    private:
        std::shared_ptr<Tileset> m_tileset;
    };
}

#endif // POKEDEV_TILESET_EDITOR_HPP