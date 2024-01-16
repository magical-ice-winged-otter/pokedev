#ifndef POKEDEV_GAME_SETTINGS_HPP
#define POKEDEV_GAME_SETTINGS_HPP

#include <filesystem>
#include "serializer.hpp"

struct GameSettings {
    void draw();

    template <class Archive>
    void serialize(Archive& archive) {
        archive(
                AUTO_NAME(projectPath)
        );
    }

    std::filesystem::path projectPath {};
};

#endif
