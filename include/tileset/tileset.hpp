#ifndef POKEDEV_TILESET_HPP
#define POKEDEV_TILESET_HPP

#include <vector>
#include <memory>

namespace PokeDev {
    class Tileset
    {
    public:
        void drawEditor();
    };

    typedef std::vector<std::shared_ptr<Tileset>> TilesetCollection;
}

#endif // POKEDEV_TILESET_HPP