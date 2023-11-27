#ifndef POKEDEV_APPLICATION_HPP
#define POKEDEV_APPLICATION_HPP

#include "game_loaders.hpp"
#include "game_settings.hpp"

/// The logic that drives the GUI and all it's supporting features.
namespace Application {

    void init();
    void shutdown();
    void render();

    extern GameLoaders loaders;
    extern GameSettings settings;

} // namespace Application

#endif // POKEDEV_APPLICATION_HPP
