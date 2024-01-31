#ifndef POKEDEV_TILESET_BROWSER_HPP
#define POKEDEV_TILESET_BROWSER_HPP

#include <vector>
#include <memory>
#include "imgui_window.hpp"
#include "tileset/tileset.hpp"
#include "tileset/tileset_editor.hpp"

namespace PokeDev {
    class TilesetBrowser : public PokeDevWindow
    {
    public:
        TilesetBrowser(std::shared_ptr<TilesetCollection> tilesetCollection);
        const char *getName() override;
        void renderWindow() override;

    private:
        std::vector<TilesetEditor> m_activeEditors;
        std::shared_ptr<TilesetCollection> m_tilesetCollection;
    };
}

#endif // POKEDEV_TILESET_BROWSER_HPP
